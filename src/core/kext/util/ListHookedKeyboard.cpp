#include <IOKit/hid/IOHIDKeys.h>
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventWatcher.hpp"
#include "FlagStatus.hpp"
#include "IOLockWrapper.hpp"
#include "KeyEventInputQueue.hpp"
#include "ListHookedKeyboard.hpp"
#include "NumHeldDownKeys.hpp"
#include "RemapClass.hpp"

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
                               unsigned int eventType_value,
                               unsigned int flags_value,
                               unsigned int key_value,
                               unsigned int charCode_value,
                               unsigned int charSet_value,
                               unsigned int origCharCode_value,
                               unsigned int origCharSet_value,
                               unsigned int keyboardType_value,
                               bool repeat,
                               AbsoluteTime ts,
                               OSObject* sender,
                               void* refcon)
    {
      if (! CommonData::eventLock) return;

      EventType eventType(eventType_value);
      Flags flags(flags_value);
      KeyCode key(key_value);
      CharCode charCode(charCode_value);
      CharSet charSet(charSet_value);
      OrigCharCode origCharCode(origCharCode_value);
      OrigCharSet origCharSet(origCharSet_value);
      KeyboardType keyboardType(keyboardType_value);

      {
        IOLockWrapper::ScopedLock lk(CommonData::eventLock);

        IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, sender);
        if (! kbd) return;

        HookedKeyboard* hk = ListHookedKeyboard::instance().get(kbd);
        if (! hk) return;

        // ------------------------------------------------------------
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
        RemapClassManager::remap_setkeyboardtype(keyboardType);
        CommonData::setcurrent_ts(ts);
        CommonData::setcurrent_vendorIDproductID(hk->getVendorID(), hk->getProductID());
        CommonData::setcurrent_keyboardType(keyboardType);

        // ------------------------------------------------------------
        // Because we handle the key repeat ourself, drop the key repeat by hardware.
        if (repeat) return;

        // ------------------------------------------------------------
        if (eventType.isKeyDownOrModifierDown(key, flags)) {
          CommonData::setcurrent_workspacedata();
        }

        // ------------------------------------------------------------
        // clear temporary_count_
        //
        // Don't call FlagStatus::set(key, flags) here.
        // If SimultaneousKeyPresses is enabled, keys may be dropped.
        // For example, Shift_L+Shift_R to Space is enabled, Shift_L and Shift_R may be dropped.
        // If we call FlagStatus::set(key, flags) here, dropped keys are kept as pushed status.
        // So, call FlagStatus::set(key, flags) after KeyEventInputQueue.
        FlagStatus::set();

        // ------------------------------------------------------------
        key.normalizeKey(flags, eventType, keyboardType);
      }

      // ------------------------------------------------------------
      KeyEventInputQueue::add(target,
                              eventType, flags, key,
                              charCode, charSet, origCharCode, origCharSet,
                              keyboardType, repeat,
                              ts, sender, refcon);
    }

    void
    hook_UpdateEventFlagsCallback(OSObject* target,
                                  unsigned flags,
                                  OSObject* sender,
                                  void* refcon)
    {
      // We don't need to get eventLock, because we do nothing here.

      // ------------------------------------------------------------
      // update device priority by calling ListHookedKeyboard::instance().get(kbd).
      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, sender);
      if (! kbd) return;

      HookedKeyboard* hk = ListHookedKeyboard::instance().get(kbd);
      if (! hk) return;

      // ------------------------------------------------------------
      Params_UpdateEventFlagsCallback::auto_ptr ptr(Params_UpdateEventFlagsCallback::alloc(flags));
      if (! ptr) return;

      Params_UpdateEventFlagsCallback& params = *ptr;
      params.log();
    }
  }

  void
  ListHookedKeyboard::hook_KeyboardEventCallback_queued(OSObject* target,
                                                        EventType eventType,
                                                        Flags flags,
                                                        KeyCode key,
                                                        CharCode charCode,
                                                        CharSet charSet,
                                                        OrigCharCode origCharCode,
                                                        OrigCharSet origCharSet,
                                                        KeyboardType keyboardType,
                                                        bool repeat,
                                                        AbsoluteTime ts,
                                                        OSObject* sender,
                                                        void* refcon)
  {
    if (! CommonData::eventLock) return;
    IOLockWrapper::ScopedLock lk(CommonData::eventLock);

    // ------------------------------------------------------------
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(eventType, flags, key,
                                                                                   charCode, charSet, origCharCode, origCharSet,
                                                                                   keyboardType, repeat));
    if (! ptr) return;
    Params_KeyboardEventCallBack& params = *ptr;

    if (params.eventType.isKeyDownOrModifierDown(params.key, params.flags)) {
      EventWatcher::on();
    }

    // ------------------------------------------------------------
    // We must call NumHeldDownKeys after inputqueue.
    // For example, when we type Command_L+S.
    //
    // (1) Command_L down (queued)
    // (2) KeyCode::S down (Command_L+S)
    // (1') dequeue Command_L down
    // (3) Command_L up
    // (4) KeyCode::S up
    // (2') dequeue KeyCode::S down
    //
    // if NumHeldDownKeys called when (4), Command_L state is reset.
    // Then (2') send KeyCode::S without Modifiers.
    NumHeldDownKeys::set(eventType, key, flags);

    Core::remap_KeyboardEventCallback(params);
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
    IOLog("KeyRemap4MacBook HookedKeyboard::initialize name = %s, device_ = %p\n", name, device_);

    return refresh();
  }

  bool
  HookedKeyboard::refresh(void)
  {
    if (! config.initialized) {
      goto restore;
    }
    if (config.general_dont_remap_thirdvendor_keyboard &&
        deviceType_ != DeviceType::APPLE_INTERNAL &&
        deviceType_ != DeviceType::APPLE_EXTERNAL) {
      goto restore;
    }
    if (config.general_dont_remap_internal &&
        deviceType_ == DeviceType::APPLE_INTERNAL) {
      goto restore;
    }
    if (config.general_dont_remap_external &&
        deviceType_ != DeviceType::APPLE_INTERNAL) {
      goto restore;
    }
    // Logitech USB Headset
    if (isEqualVendorIDProductID(DeviceVendorID(0x046d), DeviceProductID(0x0a0b))) {
      goto restore;
    }
    // Logitech Cordless Presenter
    if (config.general_dont_remap_logitech_cordless_presenter &&
        isEqualVendorIDProductID(DeviceVendorID(0x046d), DeviceProductID(0xc515))) {
      goto restore;
    }
    // Kensington Virtual Device (0x0, 0x0)
    if (isEqualVendorIDProductID(DeviceVendorID(0), DeviceProductID(0))) {
      // Note: USB Overdrive also use 0x0,0x0.
      // We allow to use USB Overdrive.
      if (deviceType_ != DeviceType::USB_OVERDRIVE) {
        goto restore;
      }
    }

#if 0
    // Apple Internal Keyboard
    if (isEqualVendorIDProductID(DeviceVendorID(0x05ac), DeviceProductID(0x21a))) {
      goto restore;
    }

    // Apple External Keyboard
    if (isEqualVendorIDProductID(DeviceProductID(0x05ac), DeviceProductID(0x0222))) {
      goto restore;
    }
#endif

    return replaceEventAction();

  restore:
    return restoreEventAction();
  }

  bool
  HookedKeyboard::terminate(void)
  {
    bool result = restoreEventAction();

    device_ = NULL;
    orig_keyboardEventAction_ = NULL;
    orig_keyboardEventTarget_ = NULL;
    orig_updateEventFlagsAction_ = NULL;
    orig_updateEventFlagsTarget_ = NULL;

    return result;
  }

  bool
  HookedKeyboard::replaceEventAction(void)
  {
    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    bool result = false;

    // ------------------------------------------------------------
    {
      KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
      if (callback != hook_KeyboardEventCallback) {
        IOLog("KeyRemap4MacBook HookedKeyboard::replaceEventAction (KeyboardEventCallback) (device_ = %p)\n", device_);

        orig_keyboardEventAction_ = callback;
        orig_keyboardEventTarget_ = kbd->_keyboardEventTarget;

        kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(hook_KeyboardEventCallback);

        result = true;
      }
    }
    {
      UpdateEventFlagsCallback callback = reinterpret_cast<UpdateEventFlagsCallback>(kbd->_updateEventFlagsAction);
      if (callback != hook_UpdateEventFlagsCallback) {
        IOLog("KeyRemap4MacBook HookedKeyboard::replaceEventAction (UpdateEventFlagsCallback) (device_ = %p)\n", device_);

        orig_updateEventFlagsAction_ = callback;
        orig_updateEventFlagsTarget_ = kbd->_updateEventFlagsTarget;

        kbd->_updateEventFlagsAction = reinterpret_cast<UpdateEventFlagsAction>(hook_UpdateEventFlagsCallback);

        result = true;
      }
    }

    return result;
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
    IOLog("KeyRemap4MacBook HookedKeyboard::restoreEventAction (device_ = %p)\n", device_);

    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(orig_keyboardEventAction_);
    kbd->_updateEventFlagsAction = reinterpret_cast<UpdateEventFlagsAction>(orig_updateEventFlagsAction_);

    orig_keyboardEventAction_ = NULL;
    orig_keyboardEventTarget_ = NULL;
    orig_updateEventFlagsAction_ = NULL;
    orig_updateEventFlagsTarget_ = NULL;

    return true;
  }
}
