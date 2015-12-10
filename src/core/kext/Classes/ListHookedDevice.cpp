#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/hid/IOHIDKeys.h>
END_IOKIT_INCLUDE;

#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedDevice.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "Params.hpp"
#include "TimerWrapper.hpp"
#include "strlcpy_utf8.hpp"

namespace org_pqrs_Karabiner {
TimerWrapper ListHookedDevice::refreshInProgressDevices_timer_;

DEFINE_WEAKPOINTER_IN_CLASS(ListHookedDevice, Item);

// ======================================================================
void ListHookedDevice::Item::setDeviceIdentifier(void) {
  if (!device_) return;

  IORegistryEntry* dev = device_;

  while (dev) {
    const OSNumber* vid = nullptr;
    vid = OSDynamicCast(OSNumber, dev->getProperty(kIOHIDVendorIDKey));

    const OSNumber* pid = nullptr;
    pid = OSDynamicCast(OSNumber, dev->getProperty(kIOHIDProductIDKey));

    if (vid && pid) {
      deviceIdentifier_.setVendor(DeviceVendor(vid->unsigned32BitValue()));
      deviceIdentifier_.setProduct(DeviceProduct(pid->unsigned32BitValue()));

      goto finish;

    } else {
      // ApplePS2Keyboard does not have ProductID,
      // so we check for Manufacturer and Product strings
      const char* name = dev->getName();

      if (name && strcmp(name, "ApplePS2Keyboard") == 0) {
        // kIOHIDManufacturerKey == "Manufacturer"
        // kIOHIDProductKey == "Product"
        const OSString* manufacturer = OSDynamicCast(OSString, dev->getProperty(kIOHIDManufacturerKey));
        const OSString* product = OSDynamicCast(OSString, dev->getProperty(kIOHIDProductKey));

        if (manufacturer && product) {
          if (manufacturer->isEqualTo("Apple") &&
              product->isEqualTo("Keyboard")) {
            deviceIdentifier_.setVendor(DeviceVendor::APPLE_COMPUTER);
            deviceIdentifier_.setProduct(DeviceProduct::APPLE_INTERNAL_KEYBOARD_TRACKPAD_0x0218);
            goto finish;
          }
        }
      }
    }

    // check parent property.
    dev = dev->getParentEntry(IORegistryEntry::getPlane(kIOServicePlane));
  }

finish:
  // Set LocationID
  if (dev) {
    const OSNumber* locationid = nullptr;
    locationid = OSDynamicCast(OSNumber, dev->getProperty(kIOHIDLocationIDKey));
    if (locationid) {
      deviceIdentifier_.setLocation(DeviceLocation(locationid->unsigned32BitValue()));
    }
  }

  IOLOG_DEBUG("HookedDevice::setVendorProductLocation device_:%p, vendor:0x%04x, product:0x%04x location:0x%04x\n",
              device_,
              deviceIdentifier_.getVendor().get(),
              deviceIdentifier_.getProduct().get(),
              deviceIdentifier_.getLocation().get());
}

void ListHookedDevice::Item::setDeviceType(void) {
  if (!device_) return;

  const char* name = nullptr;

  name = device_->getName();
  if (!name) goto finish;

  // Apple device
  if (strcmp(name, "IOHIDKeyboard") == 0 ||
      strcmp(name, "AppleADBKeyboard") == 0 ||
      strcmp(name, "IOHIDConsumer") == 0 ||
      strcmp(name, "IOHIDPointing") == 0 ||
      strcmp(name, "ApplePS2Mouse") == 0 ||
      strcmp(name, "ApplePS2Trackpad") == 0 ||
      strcmp(name, "AppleUSBGrIIITrackpad") == 0 ||
      strcmp(name, "AppleADBMouseType4") == 0) {

    deviceType_ = DeviceType::APPLE_EXTERNAL;

    // ------------------------------------------------------------
    // Judge internal device or external device.
    //
    // We judge it from a product name whether it is internal device.
    // At keyboard, we cannot use the KeyboardType,
    // because some external keyboard has the same KeyboardType as Apple internal keyboard.
    const OSString* productname = nullptr;
    productname = OSDynamicCast(OSString, device_->getProperty(kIOHIDProductKey));
    if (productname) {
      const char* pname = productname->getCStringNoCopy();
      if (pname) {
        const char* internalname = "Apple Internal ";
        if (strncmp(internalname, pname, strlen(internalname)) == 0) {
          deviceType_ = DeviceType::APPLE_INTERNAL;
        }

        const char* mikeyname = "Apple Mikey HID Driver";
        if (strcmp(mikeyname, pname) == 0) {
          deviceType_ = DeviceType::APPLE_MIKEY_HID_DRIVER;
        }
      }
    }

    goto finish;
  }

  if (strcmp(name, "ApplePS2Keyboard") == 0) {
    deviceType_ = DeviceType::APPLE_INTERNAL;
    goto finish;
  }

  // USB Overdrive
  if (strcmp(name, "OverdriveHIDKeyboard") == 0 ||
      strcmp(name, "OverdriveHIDPointer") == 0) {
    deviceType_ = DeviceType::USB_OVERDRIVE;
    goto finish;
  }

finish:
  IOLOG_DEBUG("HookedDevice::setDeviceType device_:%p, name:%s, deviceType_:%d\n",
              device_,
              name ? name : "null",
              deviceType_);
}

bool ListHookedDevice::Item::isConsumer(const char* name) {
  if (!name) return false;

  if (strcmp(name, "IOHIDConsumer") == 0) return true;
  if (strcmp(name, "com_Logitech_HISpecialKeys") == 0) return true;

  return false;
}

// ======================================================================
bool ListHookedDevice::initialize(void) {
  return true;
}

void ListHookedDevice::terminate(void) {
  list_.clear();
}

void ListHookedDevice::push_back(ListHookedDevice::Item* newp) {
  if (!newp) return;

  last_ = newp->device_;
  list_.push_back(newp);

  IOLOG_DEVEL("ListHookedDevice::push_back list_.size = %d\n", static_cast<int>(list_.size()));

  refresh();
  start_refreshInProgressDevices_timer();
}

void ListHookedDevice::erase(IOHIDevice* p) {
  ListHookedDevice::Item* item = get(p);
  if (!item) return;

  list_.erase_and_delete(item);

  IOLOG_DEVEL("ListHookedDevice::erase list_.size = %d\n", static_cast<int>(list_.size()));

  refresh();
}

ListHookedDevice::Item*
ListHookedDevice::get(const IOHIDevice* device) {
  last_ = device;

  for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    if (p->device_ == device) return p;
  }

  return nullptr;
}

ListHookedDevice::Item*
ListHookedDevice::get_replaced(void) {
  // ----------------------------------------------------------------------
  // Search a last_ device first.
  ListHookedDevice::Item* p = get(last_);
  if (p && p->isReplaced()) return p;

  // ----------------------------------------------------------------------
  // Using a first matched device.
  for (p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    if (p->isReplaced()) return p;
  }

  return nullptr;
}

void ListHookedDevice::refresh(void) {
  for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    p->refresh();
  }
}

bool ListHookedDevice::isInProgress(void) const {
  for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    if (p->inProgress_) {
      return true;
    }
  }

  return false;
}

size_t ListHookedDevice::pressingPhysicalKeysCount(void) const {
  size_t count = 0;
  for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    count += (p->pressingPhysicalKeys_).count();
  }

  return count;
}

void ListHookedDevice::clearPressingPhysicalKeysCount(void) const {
  for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    if (p->isInternalDevice()) {
      (p->pressingPhysicalKeys_).clear();
    }
  }
}

void ListHookedDevice::getDeviceInformation(BridgeDeviceInformation& out, size_t index) const {
  out.isFound = 0;
  out.manufacturer[0] = '\0';
  out.product[0] = '\0';
  out.vendorID = 0;
  out.productID = 0;
  out.locationID = 0;

  Item* p = static_cast<Item*>(list_.safe_front());

  for (;;) {
    if (!p) return;

    if (p->isReplaced()) {
      if (index == 0) {
        break;
      } else {
        --index;
      }
    }

    p = static_cast<Item*>(p->getnext());
  }

  if (!p) return;

  if (p->device_) {
    const OSString* manufacturer = OSDynamicCast(OSString, p->device_->getProperty(kIOHIDManufacturerKey));
    if (manufacturer) {
      const char* cstr = manufacturer->getCStringNoCopy();
      if (cstr) {
        pqrs::strlcpy_utf8::strlcpy(out.manufacturer, cstr, sizeof(out.manufacturer));
      }
    }

    const OSString* product = OSDynamicCast(OSString, p->device_->getProperty(kIOHIDProductKey));
    if (product) {
      const char* cstr = product->getCStringNoCopy();
      if (cstr) {
        pqrs::strlcpy_utf8::strlcpy(out.product, cstr, sizeof(out.product));
      }
    }
  }
  out.vendorID = (p->deviceIdentifier_).getVendor().get();
  out.productID = (p->deviceIdentifier_).getProduct().get();
  out.locationID = (p->deviceIdentifier_).getLocation().get();
  out.isFound = 1;
}

void ListHookedDevice::initializeAll(IOWorkLoop& workloop) {
  ListHookedKeyboard::instance().initialize();
  ListHookedConsumer::instance().initialize();
  ListHookedPointing::instance().initialize();

  refreshInProgressDevices_timer_.initialize(&workloop, nullptr, ListHookedDevice::refreshInProgressDevices_timer_callback);
}

void ListHookedDevice::terminateAll(void) {
  refreshInProgressDevices_timer_.terminate();

  ListHookedKeyboard::instance().terminate();
  ListHookedConsumer::instance().terminate();
  ListHookedPointing::instance().terminate();
}

void ListHookedDevice::refreshAll(void) {
  ListHookedKeyboard::instance().refresh();
  ListHookedConsumer::instance().refresh();
  ListHookedPointing::instance().refresh();
}

size_t ListHookedDevice::pressingPhysicalKeysCountAll(void) {
  size_t count = ListHookedKeyboard::instance().pressingPhysicalKeysCount() +
                 ListHookedConsumer::instance().pressingPhysicalKeysCount() +
                 ListHookedPointing::instance().pressingPhysicalKeysCount();
  IOLOG_DEVEL("ListHookedDevice::pressingPhysicalKeysCountAll: %ld\n", count);
  return count;
}

void ListHookedDevice::clearInternalKeyboardPressingPhysicalKeysCountAll(void) {
  ListHookedKeyboard::instance().clearPressingPhysicalKeysCount();
  ListHookedConsumer::instance().clearPressingPhysicalKeysCount();
}

void ListHookedDevice::start_refreshInProgressDevices_timer(void) {
  refreshInProgressDevices_timer_.setTimeoutMS(REFRESH_INPROGRESS_DEVICES_TIMER_INTERVAL);
}

void ListHookedDevice::refreshInProgressDevices_timer_callback(OSObject* owner, IOTimerEventSource* sender) {
  IOLOG_DEBUG("refreshInProgressDevices_timer_callback\n");

  if (ListHookedKeyboard::instance().isInProgress()) {
    ListHookedKeyboard::instance().refresh();
  }
  if (ListHookedConsumer::instance().isInProgress()) {
    ListHookedConsumer::instance().refresh();
  }
  if (ListHookedPointing::instance().isInProgress()) {
    ListHookedPointing::instance().refresh();
  }

  if (ListHookedKeyboard::instance().isInProgress() ||
      ListHookedConsumer::instance().isInProgress() ||
      ListHookedPointing::instance().isInProgress()) {
    IOLOG_DEBUG("Restarting refreshInProgressDevices_timer because some devices are in progress.\n");
    refreshInProgressDevices_timer_.setTimeoutMS(REFRESH_INPROGRESS_DEVICES_TIMER_INTERVAL);
  }
}
}
