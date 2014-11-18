#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventInputQueue.hpp"
#include "EventWatcher.hpp"
#include "FlagStatus.hpp"
#include "GlobalLock.hpp"
#include "IOLogWrapper.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "NumHeldDownKeys.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_Karabiner {
List EventInputQueue::queue_;
IntervalChecker EventInputQueue::ic_;
TimerWrapper EventInputQueue::fire_timer_;
uint64_t EventInputQueue::serialNumber_;

List EventInputQueue::BlockUntilKeyUpHander::blockedQueue_;
List EventInputQueue::BlockUntilKeyUpHander::pressingEvents_;
TimerWrapper EventInputQueue::BlockUntilKeyUpHander::blockingTimeOut_timer_;

void
EventInputQueue::initialize(IOWorkLoop& workloop) {
  ic_.begin();
  fire_timer_.initialize(&workloop, NULL, EventInputQueue::fire_timer_callback);
  serialNumber_ = 0;

  BlockUntilKeyUpHander::initialize(workloop);
}

void
EventInputQueue::terminate(void) {
  fire_timer_.terminate();

  queue_.clear();

  BlockUntilKeyUpHander::terminate();
}

void
EventInputQueue::enqueue_(const Params_KeyboardEventCallBack& p,
                          bool retainFlagStatusTemporaryCount,
                          const DeviceIdentifier& deviceIdentifier,
                          bool push_back) {
  // Because we handle the key repeat ourself, drop the key repeat.
  if (p.repeat) return;

  Item* item = new Item(p, retainFlagStatusTemporaryCount, deviceIdentifier);
  if (push_back) {
    queue_.push_back(item);
  } else {
    queue_.push_front(item);
  }
}

void
EventInputQueue::enqueue_(const Params_KeyboardSpecialEventCallback& p,
                          bool retainFlagStatusTemporaryCount,
                          const DeviceIdentifier& deviceIdentifier) {
  // Because we handle the key repeat ourself, drop the key repeat.
  if (p.repeat) return;

  queue_.push_back(new Item(p, retainFlagStatusTemporaryCount, deviceIdentifier));
}

void
EventInputQueue::enqueue_(const Params_RelativePointerEventCallback& p,
                          bool retainFlagStatusTemporaryCount,
                          const DeviceIdentifier& deviceIdentifier) {
  queue_.push_back(new Item(p, retainFlagStatusTemporaryCount, deviceIdentifier));
}

void
EventInputQueue::enqueue_(const Params_ScrollWheelEventCallback& p,
                          bool retainFlagStatusTemporaryCount,
                          const DeviceIdentifier& deviceIdentifier) {
  queue_.push_back(new Item(p, retainFlagStatusTemporaryCount, deviceIdentifier));
}

namespace {
unsigned int maxThreshold(unsigned int v1, unsigned int v2) {
  if (v1 > v2) {
    return v1;
  } else {
    return v2;
  }
}
}

void
EventInputQueue::setTimer(void) {
  Item* front = static_cast<Item*>(queue_.safe_front());
  if (!front) return;

  // ----------------------------------------
  uint32_t timeoutMS = 0;

  if (RemapClassManager::isSimultaneousKeyPressesEnabled() ||
      Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_ignore_bouncing_events)) {
    // ----------------------------------------
    // Calculate threshold
    uint32_t threshold = 0;
    const Params_Base& paramsBase = front->getParamsBase();
    bool is_key = false;
    bool is_pointingbutton = false;

    if (paramsBase.get_Params_KeyboardEventCallBack() ||
        paramsBase.get_Params_KeyboardSpecialEventCallback()) {
      is_key = true;
    }
    {
      auto p = paramsBase.get_Params_RelativePointerEventCallback();
      if (p) {
        if (p->ex_button != PointingButton::NONE) {
          is_pointingbutton = true;
        }
      }
    }

    if (RemapClassManager::isSimultaneousKeyPressesEnabled()) {
      if (is_key) {
        threshold = maxThreshold(threshold, Config::get_simultaneouskeypresses_delay());
      }
      if (is_pointingbutton) {
        threshold = maxThreshold(threshold, Config::get_simultaneouskeypresses_pointingbutton_delay());
      }
    }

    if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_ignore_bouncing_events)) {
      if (is_key) {
        threshold = maxThreshold(threshold, Config::get_ignore_bouncing_threshold_for_keyboard());
      }
      if (is_pointingbutton) {
        threshold = maxThreshold(threshold, Config::get_ignore_bouncing_threshold_for_mice());
      }
    }

    // ----------------------------------------
    // Calculate timeoutMS (== threshold - ic.getmillisec())
    uint32_t ms = (front->ic).getmillisec();

    if (ms < threshold) {
      timeoutMS = threshold - ms;
    }

    // Ignore rounding error
    if (timeoutMS > 0) {
      --timeoutMS;
    }
  }

  fire_timer_.setTimeoutMS(timeoutMS, false);
}

// ======================================================================
void
EventInputQueue::push_KeyboardEventCallback(OSObject* target,
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
                                            void* refcon) {
  GlobalLock::ScopedLock lk;
  if (!lk) return;

  Params_KeyboardEventCallBack::log(true, EventType(eventType), Flags(flags), KeyCode(key), KeyboardType(keyboardType), repeat);

  // ------------------------------------------------------------
  // Ignore unknown modifiers
  //
  // You can confirm an unknown modifier by setting key code to 255 on Seil.
  // This event also will be sent by Fn key on Leopold FC660M.
  //
  //   KeyboardEventCallback [ caught]: eventType 12, flags 0x80000000, key 0x00ff, kbdType  43, repeat = 0
  //
  // This key sends the same event at key pressing and key releasing.
  // Therefore, we cannot recognize whether key is pressed or key is released.
  // So, we have to ignore this key for NumHeldDownKeys.
  //
  if (EventType::MODIFY == EventType(eventType)) {
    if (KeyCode(key).getModifierFlag() == ModifierFlag::ZERO) {
      IOLOG_DEBUG("An unknown modifier is pressed (KeyCode:0x%x, Flags:0x%x). Ignore it.\n", key, flags);
      return;
    }
  }

  // ------------------------------------------------------------
  KeyboardType newkeyboardtype(keyboardType);
  RemapClassManager::remap_setkeyboardtype(newkeyboardtype);

  KeyCode newkey(key);
  Flags newflags(flags);
  KeyCode::normalizeKey(newkey, newflags, EventType(eventType), newkeyboardtype);

  // ------------------------------------------------------------
  Params_KeyboardEventCallBack params(EventType(eventType),
                                      newflags,
                                      newkey,
                                      CharCode(charCode),
                                      CharSet(charSet),
                                      OrigCharCode(origCharCode),
                                      OrigCharSet(origCharSet),
                                      newkeyboardtype,
                                      repeat);

  // ------------------------------------------------------------
  IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
  if (!device) return;

  ListHookedKeyboard::Item* item = static_cast<ListHookedKeyboard::Item*>(ListHookedKeyboard::instance().get(device));
  if (!item) return;

  // ------------------------------------------------------------
  // Device Hacks

  // Drop events if "Disable an internal keyboard while external keyboards are connected" is enabled.
  if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_disable_internal_keyboard_if_external_keyboard_exsits)) {
    if (item->isInternalDevice() &&
        ListHookedKeyboard::instance().isExternalDevicesConnected()) {
      return;
    }
  }

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
  if ((item->getDeviceIdentifier()).isEqualVendorProduct(DeviceVendor::LOGITECH,
                                                         DeviceProduct::LOGITECH_CORDLESS_PRESENTER)) {
    if (params.key == KeyCode::CONTROL_L) return;
    if (params.key == KeyCode::SHIFT_L) return;
  }

  // ------------------------------------------------------------
  // NumLock Hacks
  //
  // As for some keypads, NumLock is off when it was connected.
  // We need to call setAlphaLock(true) to activate a device.
  RemapClassManager::remap_forcenumlockon(item);

  // ------------------------------------------------------------
  CommonData::setcurrent_ts(ts);

  // ------------------------------------------------------------
  // Because we handle the key repeat ourself, drop the key repeat by hardware.
  if (repeat) return;

  // ------------------------------------------------------------
  bool retainFlagStatusTemporaryCount = false;
  bool push_back = true;
  enqueue_(params, retainFlagStatusTemporaryCount, item->getDeviceIdentifier(), push_back);

  setTimer();
}

void
EventInputQueue::push_UpdateEventFlagsCallback(OSObject* target,
                                               unsigned flags,
                                               OSObject* sender,
                                               void* refcon) {
  GlobalLock::ScopedLock lk;
  if (!lk) return;

  Params_UpdateEventFlagsCallback::log(true, Flags(flags));

  // ------------------------------------------------------------
  // update device priority by calling ListHookedKeyboard::instance().get(kbd).
  IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
  if (!device) return;

  ListHookedKeyboard::Item* item = static_cast<ListHookedKeyboard::Item*>(ListHookedKeyboard::instance().get(device));
  if (!item) return;

  // Don't push_back for UpdateEventFlagsCallback.
}

// ----------------------------------------------------------------------
void
EventInputQueue::push_KeyboardSpecialEventCallback(OSObject* target,
                                                   unsigned int eventType,
                                                   unsigned int flags,
                                                   unsigned int key,
                                                   unsigned int flavor,
                                                   UInt64 guid,
                                                   bool repeat,
                                                   AbsoluteTime ts,
                                                   OSObject* sender,
                                                   void* refcon) {
  GlobalLock::ScopedLock lk;
  if (!lk) return;

  Params_KeyboardSpecialEventCallback::log(true, EventType(eventType), Flags(flags), ConsumerKeyCode(key), flavor, guid, repeat);

  // ------------------------------------------------------------
  Params_KeyboardSpecialEventCallback params(EventType(eventType),
                                             Flags(flags),
                                             ConsumerKeyCode(key),
                                             flavor, guid, repeat);

  // ------------------------------------------------------------
  IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
  if (!device) return;

  ListHookedConsumer::Item* item = static_cast<ListHookedConsumer::Item*>(ListHookedConsumer::instance().get(device));
  if (!item) return;

  // ------------------------------------------------------------
  // Device Hacks

  // Drop events if "Disable an internal keyboard while external keyboards are connected" is enabled.
  if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_disable_internal_keyboard_if_external_keyboard_exsits)) {
    if (item->isInternalDevice() &&
        ListHookedKeyboard::instance().isExternalDevicesConnected()) {
      return;
    }
  }

  // ------------------------------------------------------------
  CommonData::setcurrent_ts(ts);

  // ------------------------------------------------------------
  // Because we handle the key repeat ourself, drop the key repeat by hardware.
  if (repeat) return;

  // ------------------------------------------------------------
  bool retainFlagStatusTemporaryCount = false;
  enqueue_(params, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());

  setTimer();
}

// ----------------------------------------------------------------------
void
EventInputQueue::push_RelativePointerEventCallback(OSObject* target,
                                                   int buttons_raw,
                                                   int dx,
                                                   int dy,
                                                   AbsoluteTime ts,
                                                   OSObject* sender,
                                                   void* refcon) {
  GlobalLock::ScopedLock lk;
  if (!lk) return;

  Params_RelativePointerEventCallback::log(true, Buttons(buttons_raw), dx, dy);

  // ------------------------------------------------------------
  Buttons buttons(buttons_raw);
  Buttons justPressed;
  Buttons justReleased;

  IOHIPointing* device = OSDynamicCast(IOHIPointing, sender);
  if (!device) return;

  ListHookedPointing::Item* item = static_cast<ListHookedPointing::Item*>(ListHookedPointing::instance().get(device));
  if (!item) return;

  // ------------------------------------------------------------
  CommonData::setcurrent_ts(ts);

  // ------------------------------------------------------------
  justPressed = buttons.justPressed(item->get_previousbuttons());
  justReleased = buttons.justReleased(item->get_previousbuttons());
  item->set_previousbuttons(buttons);

  // ------------------------------------------------------------
  // divide an event into button and cursormove events.
  for (int i = 0; i < ButtonStatus::MAXNUM; ++i) {
    PointingButton btn(1 << i);
    if (justPressed.isOn(btn)) {
      Params_RelativePointerEventCallback params(buttons, 0, 0, btn, true);
      bool retainFlagStatusTemporaryCount = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_lazy_modifiers_with_mouse_event);
      enqueue_(params, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());
    }
    if (justReleased.isOn(btn)) {
      Params_RelativePointerEventCallback params(buttons, 0, 0, btn, false);
      bool retainFlagStatusTemporaryCount = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_lazy_modifiers_with_mouse_event);
      enqueue_(params, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());
    }
  }
  // If (dx == 0 && dy == 0), the event is either needless event or just pressing/releasing buttons event.
  // About just pressing/releasing buttons event, we handled these in the above processes.
  // So, we can drop (dx == 0 && dy == 0) events in here.
  if (dx != 0 || dy != 0) {
    Params_RelativePointerEventCallback params(buttons, dx, dy, PointingButton::NONE, false);
    bool retainFlagStatusTemporaryCount = true;
    enqueue_(params, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());
  }

  setTimer();
}

void
EventInputQueue::push_ScrollWheelEventCallback(OSObject* target,
                                               short deltaAxis1,
                                               short deltaAxis2,
                                               short deltaAxis3,
                                               IOFixed fixedDelta1,
                                               IOFixed fixedDelta2,
                                               IOFixed fixedDelta3,
                                               SInt32 pointDelta1,
                                               SInt32 pointDelta2,
                                               SInt32 pointDelta3,
                                               SInt32 options,
                                               AbsoluteTime ts,
                                               OSObject* sender,
                                               void* refcon) {
  GlobalLock::ScopedLock lk;
  if (!lk) return;

  Params_ScrollWheelEventCallback::log(true,
                                       deltaAxis1,
                                       deltaAxis2,
                                       deltaAxis3,
                                       fixedDelta1,
                                       fixedDelta2,
                                       fixedDelta3,
                                       pointDelta1,
                                       pointDelta2,
                                       pointDelta3,
                                       options);

  // ------------------------------------------------------------
  Params_ScrollWheelEventCallback params(deltaAxis1, deltaAxis2, deltaAxis3,
                                         fixedDelta1, fixedDelta2, fixedDelta3,
                                         pointDelta1, pointDelta2, pointDelta3,
                                         options);

  // ------------------------------------------------------------
  IOHIPointing* device = OSDynamicCast(IOHIPointing, sender);
  if (!device) return;

  ListHookedPointing::Item* item = static_cast<ListHookedPointing::Item*>(ListHookedPointing::instance().get(device));
  if (!item) return;

  // ------------------------------------------------------------
  CommonData::setcurrent_ts(ts);

  // ------------------------------------------------------------
  bool retainFlagStatusTemporaryCount = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_lazy_modifiers_with_mouse_event);
  enqueue_(params, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());

  setTimer();
}

// ======================================================================
void
EventInputQueue::fire_timer_callback(OSObject* /*notuse_owner*/, IOTimerEventSource* /*notuse_sender*/) {
  // IOLOG_DEVEL("EventInputQueue::fire queue_.size = %d\n", static_cast<int>(queue_.size()));

  // ------------------------------------------------------------
  // Ignore key bouncing (chattering).
  if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_ignore_bouncing_events)) {
  retry:
    for (Item* p = static_cast<Item*>(queue_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
      // Search key up, key down event.
      bool iskeydown;
      if (p->getParamsBase().iskeydown(iskeydown) && !iskeydown) {
        Item* keyUpEvent = p;

        FromEvent fromEvent(p->getParamsBase());

        for (Item* q = static_cast<Item*>(p->getnext()); q; q = static_cast<Item*>(q->getnext())) {
          if (fromEvent.isTargetUpEvent(q->getParamsBase())) {
            break;

          } else if (fromEvent.isTargetDownEvent(q->getParamsBase())) {
            Item* keyDownEvent = q;

            uint32_t ms1 = (keyUpEvent->ic).getmillisec();
            uint32_t ms2 = (keyDownEvent->ic).getmillisec();
            uint32_t interval = ms1 - ms2;

            uint32_t threshold = 0;
            switch (fromEvent.getType()) {
            case FromEvent::Type::KEY:
            case FromEvent::Type::CONSUMER_KEY:
              threshold = Config::get_ignore_bouncing_threshold_for_keyboard();
              break;
            case FromEvent::Type::POINTING_BUTTON:
              threshold = Config::get_ignore_bouncing_threshold_for_mice();
              break;
            case FromEvent::Type::NONE:
              break;
            }

            if (interval < threshold) {
              IOLOG_INFO("Bouncing events are removed. (interval: %d, threshold: %d)\n", interval, threshold);

              queue_.erase_and_delete(keyUpEvent);
              queue_.erase_and_delete(keyDownEvent);
              goto retry;

            } else {
              IOLOG_INFO("Bouncing events? (interval: %d, threshold: %d)\n", interval, threshold);
            }
          }
        }
      }
    }
  }

  // ------------------------------------------------------------
  // handle SimultaneousKeyPresses
  while (true) {
    Item* front = static_cast<Item*>(queue_.safe_front());
    if (!front) return;

    // ------------------------------------------------------------
    // clear temporary_count_
    //
    // Don't call FlagStatus::set(key, flags) here.
    // If SimultaneousKeyPresses is enabled, keys may be dropped.
    // For example, Shift_L+Shift_R to Space is enabled, Shift_L and Shift_R may be dropped.
    // If we call FlagStatus::set(key, flags) here, dropped keys are kept as pushed status.
    // So, call FlagStatus::set(key, flags) after EventInputQueue.
    // ------------------------------------------------------------
    if (!front->retainFlagStatusTemporaryCount) {
      FlagStatus::globalFlagStatus().set();
    }

    CommonData::setcurrent_deviceIdentifier(front->deviceIdentifier);
    {
      auto params = (front->getParamsBase()).get_Params_KeyboardEventCallBack();
      if (params) {
        CommonData::setcurrent_keyboardType(params->keyboardType);
      }
    }

    bool iskeydown = false;
    if (!(front->getParamsBase()).iskeydown(iskeydown)) {
      iskeydown = true;
    }

    if (!RemapClassManager::remap_simultaneouskeypresses(iskeydown)) {
      break;
    }
  }

  // ------------------------------------------------------------
  // handle BlockUntilKeyUp
  //
  // Note:
  // We need to handle BlockUntilKeyUp after SimultaneousKeyPresses
  // in order to avoid unintended modification by SimultaneousKeyPresses.
  bool needToFire = BlockUntilKeyUpHander::doBlockUntilKeyUp();
  if (needToFire) {
    doFire();
  }

  setTimer();
}

void
EventInputQueue::doFire(void) {
  Item* p = static_cast<Item*>(queue_.safe_front());
  if (!p) return;

  {
    auto params = (p->getParamsBase()).get_Params_KeyboardEventCallBack();
    if (params) {
      if (params->ex_iskeydown) {
        EventWatcher::on();
        FlagStatus::globalFlagStatus().lazy_enable();
      } else {
        FlagStatus::globalFlagStatus().lazy_disable_if_off();
      }

      // ------------------------------------------------------------
      // We must call NumHeldDownKeys after inputqueue. (Not before queuing)
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
      //
      // ------------------------------------------------------------
      // When we press&release CapsLock, key event is fired only once.
      // (down or up depending on the state of CapsLock)
      // If we use Virtual CapsLock (remapped CapsLock) like "Change A to CapsLock",
      // the NumHeldDownKeys state is increase illegally.
      // So, we ignore Hardware CapsLock at NumHeldDownKeys.
      //
      // (1) Press Hardware CapsLock (EventType::DOWN is fired.)
      // (2) Press A (EventType::DOWN is fired.)
      // (2') (A is changed to CapsLock.)
      // (3) Release A (EventType::UP is fired.)
      // (3') (A is changed to CapsLock.)
      // (4) Press Hardware CapsLock (EventType::DOWN is fired.)
      //
      // Both (1) and (4) fire DOWN event.

      if (params->key != KeyCode::CAPSLOCK) {
        NumHeldDownKeys::set(params->ex_iskeydown ? 1 : -1);
      }

      Core::remap_KeyboardEventCallback(p->getParamsBase());
    }
  }

  {
    auto params = (p->getParamsBase()).get_Params_KeyboardSpecialEventCallback();
    if (params) {
      if (params->ex_iskeydown) {
        EventWatcher::on();
        FlagStatus::globalFlagStatus().lazy_enable();
      } else {
        FlagStatus::globalFlagStatus().lazy_disable_if_off();
      }

      // ------------------------------------------------------------
      NumHeldDownKeys::set(params->ex_iskeydown ? 1 : -1);

      Core::remap_KeyboardSpecialEventCallback(p->getParamsBase());
    }
  }

  {
    auto params = (p->getParamsBase()).get_Params_RelativePointerEventCallback();
    if (params) {
      // ------------------------------------------------------------
      // We set EventWatcher::on only when Buttons pressed.

      // ------------------------------
      // About PointingRelativeToScroll:
      //
      // If PointingRelativeToScroll is applied, we should call EventWatcher::on. (== canceling KeyOverlaidModifier)
      // When the following settings are activated,
      // Fn_Lock should not be fired if the RelativePointerEvent is happened.
      //
      // - Fn+CursorMove to ScrollWheel
      // - Fn to Fn (+ When you type Fn only, send Fn_Lock)
      //
      // But, if we call EventWatcher::on every CursorMove event, unexpected cancel occurs.
      // It's more terrible than above problem.
      //
      // Therefore, we call EventWatcher::on in PointingRelativeToScroll::remap.
      // So we don't need to call EventWatcher::on unless just buttons are pressed.

      if (params->ex_button != PointingButton::NONE &&
          params->ex_isbuttondown) {
        EventWatcher::on();
      }
      if (params->ex_button == PointingButton::NONE ||
          params->ex_isbuttondown) {
        FlagStatus::globalFlagStatus().lazy_enable();
      } else {
        FlagStatus::globalFlagStatus().lazy_disable_if_off();
      }

      // ------------------------------------------------------------
      if (params->ex_button != PointingButton::NONE) {
        NumHeldDownKeys::set(params->ex_isbuttondown ? 1 : -1);
      }

      Core::remap_RelativePointerEventCallback(p->getParamsBase());
    }
  }

  {
    auto params = (p->getParamsBase()).get_Params_ScrollWheelEventCallback();
    if (params) {
      // When "Space to Command (+ When you type Space only, send Space)" is activated,
      // user press Space and scroll wheel to input Command+ScrollWheel.
      // Then release Space, user don't intend to send Space.
      // So, we need to set EventWatcher::on here.
      EventWatcher::on();
      FlagStatus::globalFlagStatus().lazy_enable();

      Core::remap_ScrollWheelEventCallback(p->getParamsBase());
    }
  }

  CommonData::setcurrent_lastpressedphysicalkey(p->getParamsBase());

  queue_.pop_front();
  ++serialNumber_;
}

void
EventInputQueue::BlockUntilKeyUpHander::initialize(IOWorkLoop& workloop) {
  blockingTimeOut_timer_.initialize(&workloop, NULL, EventInputQueue::BlockUntilKeyUpHander::blockingTimeOut_timer_callback);
}

void
EventInputQueue::BlockUntilKeyUpHander::terminate(void) {
  blockingTimeOut_timer_.terminate();

  blockedQueue_.clear();
  pressingEvents_.clear();
}

bool
EventInputQueue::BlockUntilKeyUpHander::doBlockUntilKeyUp(void) {
  Item* front = static_cast<Item*>(queue_.safe_front());
  if (!front) return true;

  // Ignore events enqueued from blockedQueue_.
  if (front->enqueuedFrom == Item::ENQUEUED_FROM_BLOCKEDQUEUE) return true;

  // Ignore events that are not down/up events.
  // (For example, mouse cursor move events.)
  bool iskeydown = false;
  if (!(front->getParamsBase()).iskeydown(iskeydown)) return true;

  // ----------------------------------------
  // Modify pressingEvents_.
  //

  // Remove existing events.
  {
    PressingEvent* p = static_cast<PressingEvent*>(pressingEvents_.safe_front());
    for (;;) {
      if (!p) break;

      if ((p->getFromEvent()).isTargetDownEvent(front->getParamsBase()) ||
          (p->getFromEvent()).isTargetUpEvent(front->getParamsBase())) {
        p = static_cast<PressingEvent*>(pressingEvents_.erase_and_delete(p));
      } else {
        p = static_cast<PressingEvent*>(p->getnext());
      }
    }
  }

  // Add to list.
  if (iskeydown) {
    pressingEvents_.push_back(new PressingEvent(front->getParamsBase()));
  }

  // ----------------------------------------
  // Test whether pressingEvents_ are a target event of BlockUntilKeyUp.
  //

  for (PressingEvent* p = static_cast<PressingEvent*>(pressingEvents_.safe_front()); p; p = static_cast<PressingEvent*>(p->getnext())) {
    if (p->ignore()) continue;

    if (RemapClassManager::isTargetEventForBlockUntilKeyUp(p->getParamsBase())) {
      goto needToBlock;
    }
  }

  // If current event is target event, we need to block it.
  if (RemapClassManager::isTargetEventForBlockUntilKeyUp(front->getParamsBase())) {
    goto needToBlock;
  }

  endBlocking();
  return true;

needToBlock:
  // Set timeout at first.
  {
    int timeout = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_blockuntilkeyup_timeout);
    blockingTimeOut_timer_.setTimeoutMS(timeout);
  }

  // When <autogen>__BlockUntilKeyUp__ KeyCode::SPACE</autogen> is enabled:
  //
  // Case 1:
  //   * Space down
  //   * T down
  //   * T up         <- up event and event != Space
  //
  //   => Enqueue "Space down, T down, T up".
  //
  //   Note:
  //     If there is an orphan key up event,
  //     user is typing fast and we should change key events order in Case 2.
  //     So, we keep blocking.
  //
  //       * M down
  //       * Space down  (start blocking)
  //       * M up        (orphan key up event)
  //       * T down
  //       * T up
  //
  // Case 2:
  //   * Space down
  //   * T down
  //   * Space up     <- up event and event == Space
  //
  //   => Move "Space up" after "Space down".
  //      Then, Enqueue "Space down, Space up, T down".
  //

  if (!iskeydown && RemapClassManager::isTargetEventForBlockUntilKeyUp(front->getParamsBase())) {
    // Case2

    // Do not call setIgnoreToAllPressingEvents here.
    //
    // We do not need to call that here because front->p_ is already removed from pressingEvents_.
    // And if multiple __BlockUntilKeyUp__ are enabled,
    // setIgnoreToAllPressingEvents breaks other __BlockUntilKeyUp__.

    // Move up event after down event.
    FromEvent fromEvent(front->getParamsBase());
    for (Item* p = static_cast<Item*>(blockedQueue_.safe_back()); p; p = static_cast<Item*>(p->getprev())) {
      if (fromEvent.isTargetDownEvent(p->getParamsBase())) {
        if (p->getnext()) {
          blockedQueue_.insert(p->getnext(), new Item(*front));
        } else {
          blockedQueue_.push_back(new Item(*front));
        }
        goto endBlocking;
      }
    }
    // corresponded event is not found.
    blockedQueue_.push_front(new Item(*front));
    goto endBlocking;

  } else if (!iskeydown &&
             !isOrphanKeyUpEventExistsInBlockedQueue() &&
             isTargetDownEventInBlockedQueue(*front)) {
    // Case 1

    setIgnoreToAllPressingEvents();

    blockedQueue_.push_back(new Item(*front));
    goto endBlocking;
  }

  blockedQueue_.push_back(new Item(*front));
  queue_.pop_front();
  // Do not call doFire.
  return false;

endBlocking:
  queue_.pop_front();
  endBlocking();
  return true;
}

bool
EventInputQueue::BlockUntilKeyUpHander::isTargetDownEventInBlockedQueue(const Item& front) {
  FromEvent fromEvent(front.getParamsBase());

  for (Item* p = static_cast<Item*>(blockedQueue_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    if (fromEvent.isTargetDownEvent(p->getParamsBase())) {
      return true;
    }
  }

  return false;
}

bool
EventInputQueue::BlockUntilKeyUpHander::isOrphanKeyUpEventExistsInBlockedQueue(void) {
  for (Item* p = static_cast<Item*>(blockedQueue_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    bool iskeydown;
    if ((p->getParamsBase()).iskeydown(iskeydown) && !iskeydown) {
      FromEvent fromEvent(p->getParamsBase());
      bool found = false;
      for (Item* q = p; q; q = static_cast<Item*>(q->getprev())) {
        if (fromEvent.isTargetDownEvent(q->getParamsBase())) {
          found = true;
        }
      }
      if (!found) {
        return true;
      }
    }
  }

  return false;
}

void
EventInputQueue::BlockUntilKeyUpHander::endBlocking(void) {
  if (blockedQueue_.size() > 0) {
    // restore queue_
    for (;;) {
      Item* p = static_cast<Item*>(blockedQueue_.safe_back());
      if (!p) break;

      p->enqueuedFrom = Item::ENQUEUED_FROM_BLOCKEDQUEUE;
      queue_.push_front(new Item(*p));
      blockedQueue_.pop_back();
    }
  }

  blockingTimeOut_timer_.cancelTimeout();
}

void
EventInputQueue::BlockUntilKeyUpHander::setIgnoreToAllPressingEvents(void) {
  // Ignore pressingEvents_ from next.
  for (PressingEvent* p = static_cast<PressingEvent*>(pressingEvents_.safe_front()); p; p = static_cast<PressingEvent*>(p->getnext())) {
    p->setIgnore();
  }
}

void
EventInputQueue::BlockUntilKeyUpHander::blockingTimeOut_timer_callback(OSObject* owner, IOTimerEventSource* sender) {
  endBlocking();
  setIgnoreToAllPressingEvents();
  setTimer();
}
}
