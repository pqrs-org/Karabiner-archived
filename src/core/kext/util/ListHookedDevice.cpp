#include <IOKit/hid/IOHIDKeys.h>
#include "ListHookedDevice.hpp"
#include "NumHeldDownKeys.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    void
    reset(void)
    {
      NumHeldDownKeys::reset();
    }
  }

  bool
  HookedDevice::isEqualVendorIDProductID(DeviceVendorID vendorID, DeviceProductID productID) const
  {
    return vendorID_ == vendorID && productID_ == productID;
  }

  void
  HookedDevice::setVendorIDProductID(IORegistryEntry* dev)
  {
    vendorID_ = 0;
    productID_ = 0;

    while (dev) {
      const OSNumber* vid = NULL;
      vid = OSDynamicCast(OSNumber, dev->getProperty(kIOHIDVendorIDKey));

      const OSNumber* pid = NULL;
      pid = OSDynamicCast(OSNumber, dev->getProperty(kIOHIDProductIDKey));

      if (vid && pid) {
        vendorID_ = vid->unsigned32BitValue();
        productID_ = pid->unsigned32BitValue();

        goto finish;
      }

      // check parent property.
      dev = dev->getParentEntry(IORegistryEntry::getPlane(kIOServicePlane));
    }

  finish:
    IOLog("KeyRemap4MacBook HookedDevice::setVendorIDProductID (device = %p, vendorID_ = 0x%x, productID_ = 0x%x)\n", dev, vendorID_, productID_);
  }

  void
  HookedDevice::setDeviceType(IOHIDevice* dev)
  {
    const char* name = NULL;

    deviceType_ = DeviceType::UNKNOWN;

    if (! dev) goto finish;

    name = dev->getName();
    if (! name) goto finish;

    // Apple device
    if (strcmp(name, "IOHIDKeyboard")         == 0 ||
        strcmp(name, "AppleADBKeyboard")      == 0 ||
        strcmp(name, "IOHIDConsumer")         == 0 ||
        strcmp(name, "IOHIDPointing")         == 0 ||
        strcmp(name, "AppleUSBGrIIITrackpad") == 0 ||
        strcmp(name, "AppleADBMouseType4")    == 0) {

      deviceType_ = DeviceType::APPLE_EXTERNAL;

      // ------------------------------------------------------------
      // Judge internal device or external device.
      //
      // We judge it from a product name whether it is internal device.
      // At keyboard, we cannot use the KeyboardType,
      // because some external keyboard has the same KeyboardType as Apple internal keyboard.
      const OSString* productname = NULL;
      productname = OSDynamicCast(OSString, dev->getProperty(kIOHIDProductKey));
      if (productname) {
        const char* pname = productname->getCStringNoCopy();
        if (pname) {
          const char* internalname = "Apple Internal ";
          if (strncmp(internalname, pname, strlen(internalname)) == 0) {
            deviceType_ = DeviceType::APPLE_INTERNAL;
          }
        }
      }

      goto finish;
    }

    // USB Overdrive
    if (strcmp(name, "OverdriveHIDKeyboard") == 0 ||
        strcmp(name, "OverdriveHIDPointer")  == 0) {
      deviceType_ = DeviceType::USB_OVERDRIVE;
      goto finish;
    }

  finish:
    IOLog("KeyRemap4MacBook HookedDevice::setDeviceType (device = %p, name = %s, deviceType_ = %d)\n",
          dev,
          name ? name : "null",
          deviceType_);
  }

  bool
  HookedDevice::isConsumer(const char* name)
  {
    if (! name) return false;

    if (strcmp(name, "IOHIDConsumer") == 0) return true;
    if (strcmp(name, "com_Logitech_HISpecialKeys") == 0) return true;

    return false;
  }

  // ----------------------------------------------------------------------
  bool
  ListHookedDevice::initialize(void)
  {
    lock_ = IOLockWrapper::alloc();
    if (! lock_) return false;

    return true;
  }

  bool
  ListHookedDevice::append(IOHIDevice* device)
  {
    if (! lock_) return false;
    IOLockWrapper::ScopedLock lk(lock_);

    last_ = device;

    for (int i = 0; i < MAXNUM; ++i) {
      HookedDevice* p = getItem(i);
      if (! p) continue;
      if (p->get()) continue;

      IOLog("KeyRemap4MacBook ListHookedDevice::append (device = %p, slot = %d)\n", device, i);

      p->setVendorIDProductID(device);
      p->setDeviceType(device);
      p->initialize(device);

      // Call reset whenever the device status is changed.
      reset();

      return true;
    }

    return false;
  }

  void
  ListHookedDevice::terminate(void)
  {
    if (! lock_) return;

    {
      IOLockWrapper::ScopedLock lk(lock_);

      last_ = NULL;

      for (int i = 0; i < MAXNUM; ++i) {
        HookedDevice* p = getItem(i);
        if (! p) continue;

        p->terminate();
      }
    }

    reset();

    IOLockWrapper::free(lock_);
  }

  bool
  ListHookedDevice::terminate(const IOHIDevice* device)
  {
    if (! lock_) return false;
    IOLockWrapper::ScopedLock lk(lock_);

    HookedDevice* p = get_nolock(device);
    if (! p) return false;

    p->terminate();
    p->vendorID_ = 0;
    p->productID_ = 0;
    p->device_ = NULL;

    reset();
    return true;
  }

  HookedDevice*
  ListHookedDevice::get_nolock(const IOHIDevice* device)
  {
    last_ = device;

    if (! device) return NULL;

    for (int i = 0; i < MAXNUM; ++i) {
      HookedDevice* p = getItem(i);
      if (! p) continue;

      if (p->get() == device) return p;
    }
    return NULL;
  }

  HookedDevice*
  ListHookedDevice::get(const IOHIDevice* device)
  {
    if (! lock_) return NULL;
    IOLockWrapper::ScopedLock lk(lock_);

    return get_nolock(device);
  }

  HookedDevice*
  ListHookedDevice::get(void)
  {
    if (! lock_) return NULL;
    IOLockWrapper::ScopedLock lk(lock_);

    // ----------------------------------------------------------------------
    // Search a replaced device first.
    HookedDevice* p = get_nolock(last_);
    if (p && p->isReplaced()) return p;

    for (int i = 0; i < MAXNUM; ++i) {
      p = getItem(i);
      if (p && p->isReplaced()) return p;
    }

    // Search others.
    p = get_nolock(last_);
    if (p) return p;

    for (int i = 0; i < MAXNUM; ++i) {
      p = getItem(i);
      if (p) return p;
    }

    return NULL;
  }

  void
  ListHookedDevice::refresh(void)
  {
    if (! lock_) return;
    IOLockWrapper::ScopedLock lk(lock_);

    for (int i = 0; i < MAXNUM; ++i) {
      HookedDevice* p = getItem(i);
      if (! p) continue;

      if (p->refresh()) {
        // reset if any event actions are replaced.
        reset();
      }
    }
  }
}
