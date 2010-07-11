#include <IOKit/hid/IOHIDKeys.h>
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventWatcher.hpp"
#include "FlagStatus.hpp"
#include "IOLockWrapper.hpp"
#include "EventInputQueue.hpp"
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

  ListHookedKeyboard::Item::Item(IOHIDevice* p) : ListHookedDevice::Item(p),
                                                  orig_keyboardEventAction_(NULL),
                                                  orig_keyboardEventTarget_(NULL),
                                                  orig_updateEventFlagsAction_(NULL),
                                                  orig_updateEventFlagsTarget_(NULL),
                                                  replacerestore_lock_(NULL)
  {
    replacerestore_lock_ = IOLockWrapper::alloc();
  }

  ListHookedKeyboard::Item::~Item(void)
  {
    IOLOG_INFO("ListHookedKeyboard::Item::~Item()\n");
    IOLockWrapper::free(replacerestore_lock_);
    restoreEventAction();
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
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType(eventType),
                                                                                     Flags(flags),
                                                                                     KeyCode(key),
                                                                                     CharCode(charCode),
                                                                                     CharSet(charSet),
                                                                                     OrigCharCode(origCharCode),
                                                                                     OrigCharSet(origCharSet),
                                                                                     KeyboardType(keyboardType),
                                                                                     repeat));
      if (! ptr) return;
      Params_KeyboardEventCallBack& params = *ptr;

      {
        if (! CommonData::eventLock) return;
        IOLockWrapper::ScopedLock lk(CommonData::eventLock);

        IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, sender);
        if (! kbd) return;

        ListHookedKeyboard::Item* hk = ListHookedKeyboard::instance().get(kbd);
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
          if (params.key == KeyCode::CONTROL_L) return;
          if (params.key == KeyCode::SHIFT_L) return;
        }

        // ------------------------------------------------------------
        RemapClassManager::remap_setkeyboardtype(params.keyboardType);
        CommonData::setcurrent_ts(ts);
        CommonData::setcurrent_vendorIDproductID(hk->getVendorID(), hk->getProductID());
        CommonData::setcurrent_keyboardType(params.keyboardType);

        // ------------------------------------------------------------
        // Because we handle the key repeat ourself, drop the key repeat by hardware.
        if (repeat) return;

        // ------------------------------------------------------------
        if (params.eventType.isKeyDownOrModifierDown(params.key, params.flags)) {
          CommonData::setcurrent_workspacedata();
        }

        // ------------------------------------------------------------
        // clear temporary_count_
        //
        // Don't call FlagStatus::set(key, flags) here.
        // If SimultaneousKeyPresses is enabled, keys may be dropped.
        // For example, Shift_L+Shift_R to Space is enabled, Shift_L and Shift_R may be dropped.
        // If we call FlagStatus::set(key, flags) here, dropped keys are kept as pushed status.
        // So, call FlagStatus::set(key, flags) after EventInputQueue.
        FlagStatus::set();

        // ------------------------------------------------------------
        params.key.normalizeKey(params.flags, params.eventType, params.keyboardType);
      }

      // ------------------------------------------------------------
      EventInputQueue::push(params);
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

      ListHookedKeyboard::Item* hk = ListHookedKeyboard::instance().get(kbd);
      if (! hk) return;

      // ------------------------------------------------------------
      Params_UpdateEventFlagsCallback::auto_ptr ptr(Params_UpdateEventFlagsCallback::alloc(flags));
      if (! ptr) return;

      Params_UpdateEventFlagsCallback& params = *ptr;
      params.log();
    }
  }

  void
  ListHookedKeyboard::hook_KeyboardEventCallback_queued(Params_KeyboardEventCallBack& params)
  {
    if (! CommonData::eventLock) return;
    IOLockWrapper::ScopedLock lk(CommonData::eventLock);

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
    NumHeldDownKeys::set(params.eventType, params.key, params.flags);

    Core::remap_KeyboardEventCallback(params);
  }

  // ======================================================================
  bool
  ListHookedKeyboard::Item::refresh_callback(void)
  {
    if (! device_) goto restore;

    {
      const char* name = device_->getName();
      if (! name) goto restore;

      if (ListHookedDevice::Item::isConsumer(name)) goto restore;
    }

    // ------------------------------------------------------------
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
  ListHookedKeyboard::Item::replaceEventAction(void)
  {
    IOLockWrapper::ScopedLock lk(replacerestore_lock_);

    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    bool result = false;

    // ------------------------------------------------------------
    {
      KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
      if (callback != hook_KeyboardEventCallback) {
        IOLOG_INFO("HookedKeyboard::replaceEventAction (KeyboardEventCallback) device_:%p\n", device_);

        orig_keyboardEventAction_ = callback;
        orig_keyboardEventTarget_ = kbd->_keyboardEventTarget;

        kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(hook_KeyboardEventCallback);

        result = true;
      }
    }
    {
      UpdateEventFlagsCallback callback = reinterpret_cast<UpdateEventFlagsCallback>(kbd->_updateEventFlagsAction);
      if (callback != hook_UpdateEventFlagsCallback) {
        IOLOG_INFO("HookedKeyboard::replaceEventAction (UpdateEventFlagsCallback) device_:%p\n", device_);

        orig_updateEventFlagsAction_ = callback;
        orig_updateEventFlagsTarget_ = kbd->_updateEventFlagsTarget;

        kbd->_updateEventFlagsAction = reinterpret_cast<UpdateEventFlagsAction>(hook_UpdateEventFlagsCallback);

        result = true;
      }
    }

    return result;
  }

  bool
  ListHookedKeyboard::Item::restoreEventAction(void)
  {
    IOLockWrapper::ScopedLock lk(replacerestore_lock_);

    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
    if (callback != hook_KeyboardEventCallback) return false;

    // ----------------------------------------
    IOLOG_INFO("HookedKeyboard::restoreEventAction device_:%p\n", device_);

    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(orig_keyboardEventAction_);
    kbd->_updateEventFlagsAction = reinterpret_cast<UpdateEventFlagsAction>(orig_updateEventFlagsAction_);

    orig_keyboardEventAction_ = NULL;
    orig_keyboardEventTarget_ = NULL;
    orig_updateEventFlagsAction_ = NULL;
    orig_updateEventFlagsTarget_ = NULL;

    return true;
  }

  // ======================================================================
  void
  ListHookedKeyboard::Item::apply(const Params_KeyboardEventCallBack& params)
  {
    if (params.key >= KeyCode::VK__BEGIN__) {
      // Invalid keycode
      IOLOG_ERROR("ListHookedKeyboard::Item::apply invalid key:%d\n", params.key.get());
      return;
    }
    if (params.eventType == EventType::MODIFY && ! params.key.isModifier()) {
      // Invalid modifierkeycode
      IOLOG_ERROR("ListHookedKeyboard::Item::apply invalid modifierkey:%d\n", params.key.get());
      return;
    }
    if (params.flags.isVirtualModifiersOn()) {
      IOLOG_ERROR("%s invalid flags:%d\n", __PRETTY_FUNCTION__, params.flags.get());
      return;
    }

    // ------------------------------------------------------------
    if (RemapClassManager::remap_dropkeyafterremap(params)) return;

    // ------------------------------------------------------------
    KeyboardEventCallback callback = orig_keyboardEventAction_;
    if (! callback) return;

    OSObject* target = orig_keyboardEventTarget_;
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, device_);
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    params.log("sending");
    callback(target, params.eventType.get(), params.flags.get(), params.key.get(),
             params.charCode.get(), params.charSet.get(), params.origCharCode.get(), params.origCharSet.get(),
             params.keyboardType.get(), params.repeat, ts, sender, refcon);

    CommonData::setcurrent_keyboardType(params.keyboardType);

    // --------------------
    if (params.eventType == EventType::DOWN) {
      FlagStatus::sticky_clear();
    }

    // --------------------
    // handle CapsLock LED
    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (kbd) {
      int led = kbd->getLEDStatus();
      if (config.general_capslock_led_hack) {
        if (led == 0) {
          kbd->setAlphaLockFeedback(true);
        }
      } else {
        if (params.flags.isOn(ModifierFlag::CAPSLOCK)) {
          if (led == 0) {
            kbd->setAlphaLockFeedback(true);
          }
        } else {
          if (led != 0) {
            kbd->setAlphaLockFeedback(false);
          }
        }
      }
    }
  }

  void
  ListHookedKeyboard::Item::apply(const Params_UpdateEventFlagsCallback& params)
  {
    if (params.flags.isVirtualModifiersOn()) {
      IOLOG_ERROR("%s invalid flags:%d\n", __PRETTY_FUNCTION__, params.flags.get());
      return;
    }

    // ------------------------------------------------------------
    UpdateEventFlagsCallback callback = orig_updateEventFlagsAction_;
    if (! callback) return;

    OSObject* target = orig_updateEventFlagsTarget_;
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, device_);
    if (! sender) return;

    OSObject* refcon = NULL;

    params.log("sending");
    callback(target, params.flags.get(), sender, refcon);
  }

  void
  ListHookedKeyboard::apply(const Params_KeyboardEventCallBack& params)
  {
    ListHookedKeyboard::Item* p = get();
    if (p) {
      p->apply(params);
    }
  }

  void
  ListHookedKeyboard::apply(const Params_UpdateEventFlagsCallback& params)
  {
    ListHookedKeyboard::Item* p = get();
    if (p) {
      p->apply(params);
    }
  }
}
