#include <IOKit/hid/IOHIDKeys.h>
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventWatcher.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedKeyboard.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    ListHookedKeyboard listHookedKeyboard;
  }

  ListHookedKeyboard&
  ListHookedKeyboard::instance(void)
  {
    return listHookedKeyboard;
  }

  // ----------------------------------------------------------------------
  namespace {
    void
    hook_KeyboardEventCallback(OSObject* target,
                               unsigned int eventType,
                               unsigned int flags,
                               unsigned int key,
                               unsigned int charCode,
                               unsigned int charSet,
                               unsigned int origCharCode,
                               unsigned int origCharSet,
                               unsigned int keyboardType,
                               bool repeat,
                               AbsoluteTime ts,
                               OSObject* sender,
                               void* refcon)
    {
      if (! CommonData::eventLock) return;
      IOLockWrapper::ScopedLock lk(CommonData::eventLock);

      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, sender);
      if (! kbd) return;

      HookedKeyboard* hk = ListHookedKeyboard::instance().get(kbd);
      if (! hk) return;

      // Logitech Cordless Presenter (LCP) Hack
      //
      // When an LCP is first plugged in, it will send a CONTROL_L down event
      // when the first pageup/pagedown key is pressed without sending a corresponding
      // up event -- effectively rendering the device (and the Mac) useless until it is
      // unplugged from the system.
      //
      // Similarly, when the volume keys are first pressed, a SHIFT_L down event
      // is generated, with now up event.
      //
      // This code effectively throws these events away if they are received from an LCP.
      //
      // *** LCP has 6 keys (Page Up, Page Down, a 'B' key, an 'Esc' key, and volume up / down keys). ***
      // *** So, we can drop CONTROL_L and SHIFT_L without a problem. ***
      if (hk->isEqualVendorIDProductID(DeviceVendorID(0x046d), DeviceProductID(0xc515))) {
        if (KeyCode::CONTROL_L == key) return;
        if (KeyCode::SHIFT_L == key) return;
      }

      // ------------------------------------------------------------
#include "../config/output/include.remapcode_keyboardtype.hpp"
      CommonData::setcurrent_ts(ts);
      CommonData::setcurrent_vendorIDproductID(hk->getVendorID(), hk->getProductID());
      CommonData::setcurrent_keyboardType(keyboardType);

      // ------------------------------------------------------------
      // Because we handle the key repeat ourself, drop the key repeat by hardware.
      if (repeat) return;

      // ------------------------------------------------------------
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType(eventType), Flags(flags), KeyCode(key),
                                                                                     CharCode(charCode), CharSet(charSet), OrigCharCode(origCharCode), OrigCharSet(origCharSet),
                                                                                     KeyboardType(keyboardType), repeat));
      if (! ptr) return;
      Params_KeyboardEventCallBack& params = *ptr;

      if (params.eventType.isKeyDownOrModifierDown(params.key, params.flags)) {
        EventWatcher::on();
      }

      Core::remap_KeyboardEventCallback(params);
    }
  }

  bool
  HookedKeyboard::initialize(IOHIDevice* d)
  {
    if (! d) return false;

    const char* name = d->getName();
    if (! name) return false;

    if (HookedDevice::isConsumer(name)) return false;

    // ------------------------------------------------------------
    device_ = d;
    IOLog("KeyRemap4MacBook HookedKeyboard::initialize name = %s, device_ = 0x%p\n", name, device_);

    if (strcmp(name, "IOHIDKeyboard") == 0 ||
        strcmp(name, "AppleADBKeyboard") == 0) {
      isAppleDriver_ = true;
    } else {
      isAppleDriver_ = false;
    }

    // ------------------------------------------------------------
    // set isInternalKeyboard_
    //
    // We judge it from a product name whether it is internal keyboard.
    // We cannot use the KeyboardType,
    // because some external keyboard has the same KeyboardType as Apple internal keyboard.

    const OSString* productname = NULL;
    productname = OSDynamicCast(OSString, device_->getProperty(kIOHIDProductKey));
    if (productname) {
      const char* pname = productname->getCStringNoCopy();
      if (pname) {
        const char* internalname = "Apple Internal ";
        if (strncmp(internalname, pname, strlen(internalname)) == 0) {
          isInternalKeyboard_ = true;
        }
      }
    }

    return refresh();
  }

  bool
  HookedKeyboard::refresh(void)
  {
    if (! config.initialized) {
      return restoreEventAction();
    }
    if (! isAppleDriver_ && config.general_dont_remap_thirdvendor_keyboard) {
      return restoreEventAction();
    }
    if (isInternalKeyboard_) {
      if (config.general_dont_remap_internal) {
        return restoreEventAction();
      }
    } else {
      if (config.general_dont_remap_external) {
        return restoreEventAction();
      }
    }

    return replaceEventAction();
  }

  bool
  HookedKeyboard::terminate(void)
  {
    bool result = restoreEventAction();

    device_ = NULL;
    orig_keyboardEventAction_ = NULL;
    orig_keyboardEventTarget_ = NULL;

    return result;
  }

  bool
  HookedKeyboard::replaceEventAction(void)
  {
    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
    if (callback == hook_KeyboardEventCallback) return false;

    // ------------------------------------------------------------
    IOLog("KeyRemap4MacBook HookedKeyboard::replaceEventAction (device_ = 0x%p)\n", device_);

    orig_keyboardEventAction_ = callback;
    orig_keyboardEventTarget_ = kbd->_keyboardEventTarget;

    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(hook_KeyboardEventCallback);

    return true;
  }

  bool
  HookedKeyboard::restoreEventAction(void)
  {
    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
    if (callback != hook_KeyboardEventCallback) return false;

    // ----------------------------------------
    IOLog("KeyRemap4MacBook HookedKeyboard::restoreEventAction (device_ = 0x%p)\n", device_);

    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(orig_keyboardEventAction_);

    orig_keyboardEventAction_ = NULL;
    orig_keyboardEventTarget_ = NULL;

    return true;
  }
}
