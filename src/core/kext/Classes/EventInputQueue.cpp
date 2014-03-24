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

namespace org_pqrs_KeyRemap4MacBook {
  List* EventInputQueue::queue_ = NULL;
  IntervalChecker EventInputQueue::ic_;
  TimerWrapper EventInputQueue::fire_timer_;

  void
  EventInputQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new List();
    ic_.begin();
    fire_timer_.initialize(&workloop, NULL, EventInputQueue::fire_timer_callback);
  }

  void
  EventInputQueue::terminate(void)
  {
    fire_timer_.terminate();

    if (queue_) {
      delete queue_;
    }
  }

  uint32_t
  EventInputQueue::calcdelay(DelayType type)
  {
    // if no SimultaneousKeyPresses is enabled, fire immediately.
    if (! RemapClassManager::isEventInputQueueDelayEnabled()) {
      return IMMEDIATELY_DELAY;
    }

    uint32_t ms = ic_.getmillisec();
    uint32_t delay = 0;
    switch (type) {
      case DELAY_TYPE_KEY:
        delay = Config::get_simultaneouskeypresses_delay();
        break;
      case DELAY_TYPE_POINTING_BUTTON:
        delay = Config::get_simultaneouskeypresses_pointingbutton_delay();
        break;
    }
    if (delay > ms) delay = ms;  // min(ms, delay)
    if (delay < MIN_DELAY) delay = MIN_DELAY;  // max(MIN_DELAY, delay)
    ic_.begin();
    return delay;
  }

  void
  EventInputQueue::enqueue_(const Params_KeyboardEventCallBack& p,
                            bool retainFlagStatusTemporaryCount,
                            const DeviceIdentifier& deviceIdentifier,
                            bool push_back)
  {
    if (! queue_) return;

    // Because we handle the key repeat ourself, drop the key repeat.
    if (p.repeat) return;

    // --------------------
    uint32_t delay = calcdelay(DELAY_TYPE_KEY);
    Item* item = new Item(p, retainFlagStatusTemporaryCount, deviceIdentifier, delay);
    if (push_back) {
      queue_->push_back(item);
    } else {
      queue_->push_front(item);
    }
  }

  void
  EventInputQueue::enqueue_(const Params_KeyboardSpecialEventCallback& p,
                            bool retainFlagStatusTemporaryCount,
                            const DeviceIdentifier& deviceIdentifier)
  {
    if (! queue_) return;

    // Because we handle the key repeat ourself, drop the key repeat.
    if (p.repeat) return;

    // --------------------
    uint32_t delay = calcdelay(DELAY_TYPE_KEY);
    queue_->push_back(new Item(p, retainFlagStatusTemporaryCount, deviceIdentifier, delay));
  }

  void
  EventInputQueue::enqueue_(const Params_RelativePointerEventCallback& p,
                            bool retainFlagStatusTemporaryCount,
                            const DeviceIdentifier& deviceIdentifier)
  {
    if (! queue_) return;

    // --------------------
    uint32_t delay = calcdelay(DELAY_TYPE_POINTING_BUTTON);
    queue_->push_back(new Item(p, retainFlagStatusTemporaryCount, deviceIdentifier, delay));
  }

  void
  EventInputQueue::enqueue_(const Params_ScrollWheelEventCallback& p,
                            bool retainFlagStatusTemporaryCount,
                            const DeviceIdentifier& deviceIdentifier)
  {
    if (! queue_) return;

    // --------------------
    uint32_t delay = calcdelay(DELAY_TYPE_POINTING_BUTTON);
    queue_->push_back(new Item(p, retainFlagStatusTemporaryCount, deviceIdentifier, delay));
  }

  void
  EventInputQueue::setTimer(void)
  {
    if (! queue_) return;

    Item* front = static_cast<Item*>(queue_->front());
    if (front) {
      fire_timer_.setTimeoutMS(front->delayMS, false);
    }
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
                                              void* refcon)
  {
    GlobalLock::ScopedLock lk;
    if (! lk) return;

    Params_KeyboardEventCallBack::log(true, EventType(eventType), Flags(flags), KeyCode(key), KeyboardType(keyboardType), repeat);

    // ------------------------------------------------------------
    // Ignore unknown modifiers
    //
    // You can confirm an unknown modifier by setting key code to 255 on PCKeyboardHack.
    // This event also will be sent by Fn key on Leopold FC660M.
    //
    //   KeyboardEventCallback [ caught]: eventType 12, flags 0x80000000, key 0x00ff, kbdType  43, repeat = 0
    //
    // This key sends the same event at key pressing and key releasing.
    // Therefore, we cannot recognize whether key is pressed or key is released.
    // So, we have to ignore this key for NumHeldDownKeys.
    //
    if (EventType::MODIFY == EventType(eventType)) {
      if (KeyCode(key).getModifierFlag() == ModifierFlag::NONE) {
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
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType(eventType),
                                                                                   newflags,
                                                                                   newkey,
                                                                                   CharCode(charCode),
                                                                                   CharSet(charSet),
                                                                                   OrigCharCode(origCharCode),
                                                                                   OrigCharSet(origCharSet),
                                                                                   newkeyboardtype,
                                                                                   repeat));
    if (! ptr) return;
    Params_KeyboardEventCallBack& params = *ptr;

    // ------------------------------------------------------------
    IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
    if (! device) return;

    ListHookedKeyboard::Item* item = static_cast<ListHookedKeyboard::Item*>(ListHookedKeyboard::instance().get(device));
    if (! item) return;

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
                                                 void* refcon)
  {
    GlobalLock::ScopedLock lk;
    if (! lk) return;

    Params_UpdateEventFlagsCallback::log(true, Flags(flags));

    // ------------------------------------------------------------
    // update device priority by calling ListHookedKeyboard::instance().get(kbd).
    IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
    if (! device) return;

    ListHookedKeyboard::Item* item = static_cast<ListHookedKeyboard::Item*>(ListHookedKeyboard::instance().get(device));
    if (! item) return;

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
                                                     void* refcon)
  {
    GlobalLock::ScopedLock lk;
    if (! lk) return;

    Params_KeyboardSpecialEventCallback::log(true, EventType(eventType), Flags(flags), ConsumerKeyCode(key), flavor, guid, repeat);

    // ------------------------------------------------------------
    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(EventType(eventType), Flags(flags), ConsumerKeyCode(key),
                                                                                                 flavor, guid, repeat));
    if (! ptr) return;
    Params_KeyboardSpecialEventCallback& params = *ptr;

    // ------------------------------------------------------------
    IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
    if (! device) return;

    ListHookedConsumer::Item* item = static_cast<ListHookedConsumer::Item*>(ListHookedConsumer::instance().get(device));
    if (! item) return;

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
                                                     void* refcon)
  {
    GlobalLock::ScopedLock lk;
    if (! lk) return;

    Params_RelativePointerEventCallback::log(true, Buttons(buttons_raw), dx, dy);

    // ------------------------------------------------------------
    Buttons buttons(buttons_raw);
    Buttons justPressed;
    Buttons justReleased;

    IOHIPointing* device = OSDynamicCast(IOHIPointing, sender);
    if (! device) return;

    ListHookedPointing::Item* item = static_cast<ListHookedPointing::Item*>(ListHookedPointing::instance().get(device));
    if (! item) return;

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
        Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, 0, 0, btn, true));
        if (! ptr) return;
        bool retainFlagStatusTemporaryCount = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_lazy_modifiers_with_mouse_event);
        enqueue_(*ptr, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());
      }
      if (justReleased.isOn(btn)) {
        Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, 0, 0, btn, false));
        if (! ptr) return;
        bool retainFlagStatusTemporaryCount = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_lazy_modifiers_with_mouse_event);
        enqueue_(*ptr, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());
      }
    }
    // If (dx == 0 && dy == 0), the event is either needless event or just pressing/releasing buttons event.
    // About just pressing/releasing buttons event, we handled these in the above processes.
    // So, we can drop (dx == 0 && dy == 0) events in here.
    if (dx != 0 || dy != 0) {
      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, dx, dy, PointingButton::NONE, false));
      if (! ptr) return;
      bool retainFlagStatusTemporaryCount = true;
      enqueue_(*ptr, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());
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
                                                 void* refcon)
  {
    GlobalLock::ScopedLock lk;
    if (! lk) return;

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
    Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(deltaAxis1, deltaAxis2, deltaAxis3,
                                                                                         fixedDelta1, fixedDelta2, fixedDelta3,
                                                                                         pointDelta1, pointDelta2, pointDelta3,
                                                                                         options));
    if (! ptr) return;
    Params_ScrollWheelEventCallback& params = *ptr;

    // ------------------------------------------------------------
    IOHIPointing* device = OSDynamicCast(IOHIPointing, sender);
    if (! device) return;

    ListHookedPointing::Item* item = static_cast<ListHookedPointing::Item*>(ListHookedPointing::instance().get(device));
    if (! item) return;

    // ------------------------------------------------------------
    CommonData::setcurrent_ts(ts);

    // ------------------------------------------------------------
    bool retainFlagStatusTemporaryCount = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_lazy_modifiers_with_mouse_event);
    enqueue_(params, retainFlagStatusTemporaryCount, item->getDeviceIdentifier());

    setTimer();
  }

  // ======================================================================
  void
  EventInputQueue::fire_timer_callback(OSObject* /*notuse_owner*/, IOTimerEventSource* /*notuse_sender*/)
  {
    if (! queue_) return;

    // IOLOG_DEVEL("EventInputQueue::fire queue_->size = %d\n", static_cast<int>(queue_->size()));

    // ------------------------------------------------------------
    // handle SimultaneousKeyPresses
    do {
      Item* front = static_cast<Item*>(queue_->front());
      if (! front) return;

      // ------------------------------------------------------------
      // clear temporary_count_
      //
      // Don't call FlagStatus::set(key, flags) here.
      // If SimultaneousKeyPresses is enabled, keys may be dropped.
      // For example, Shift_L+Shift_R to Space is enabled, Shift_L and Shift_R may be dropped.
      // If we call FlagStatus::set(key, flags) here, dropped keys are kept as pushed status.
      // So, call FlagStatus::set(key, flags) after EventInputQueue.
      // ------------------------------------------------------------
      if (! front->retainFlagStatusTemporaryCount) {
        FlagStatus::set();
      }

      CommonData::setcurrent_deviceIdentifier(front->deviceIdentifier);
      {
        Params_KeyboardEventCallBack* params = (front->params).get_Params_KeyboardEventCallBack();
        if (params) {
          CommonData::setcurrent_keyboardType(params->keyboardType);
        }
      }

    } while (RemapClassManager::remap_simultaneouskeypresses());

    // ------------------------------------------------------------
    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;

    switch (p->params.type) {
      case ParamsUnion::KEYBOARD:
      {
        Params_KeyboardEventCallBack* params = (p->params).get_Params_KeyboardEventCallBack();
        if (params) {
          if (params->ex_iskeydown) {
            EventWatcher::on();
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

          Core::remap_KeyboardEventCallback(p->params);
        }
        break;
      }

      case ParamsUnion::KEYBOARD_SPECIAL:
      {
        Params_KeyboardSpecialEventCallback* params = (p->params).get_Params_KeyboardSpecialEventCallback();
        if (params) {
          if (params->ex_iskeydown) {
            EventWatcher::on();
          }

          // ------------------------------------------------------------
          NumHeldDownKeys::set(params->ex_iskeydown ? 1 : -1);

          Core::remap_KeyboardSpecialEventCallback(p->params);
        }
        break;
      }

      case ParamsUnion::RELATIVE_POINTER:
      {
        Params_RelativePointerEventCallback* params = (p->params).get_Params_RelativePointerEventCallback();
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

          // ------------------------------------------------------------
          if (params->ex_button != PointingButton::NONE) {
            NumHeldDownKeys::set(params->ex_isbuttondown ? 1 : -1);
          }

          Core::remap_RelativePointerEventCallback(p->params);
        }
        break;
      }

      case ParamsUnion::SCROLL_WHEEL:
      {
        Params_ScrollWheelEventCallback* params = (p->params).get_Params_ScrollWheelEventCallback();
        if (params) {
          // When "Space to Command (+ When you type Space only, send Space)" is activated,
          // user press Space and scroll wheel to input Command+ScrollWheel.
          // Then release Space, user don't intend to send Space.
          // So, we need to set EventWatcher::on here.
          EventWatcher::on();

          Core::remap_ScrollWheelEventCallback(p->params);
        }
        break;
      }

      default:
        IOLOG_ERROR("%s unkown type\n", __PRETTY_FUNCTION__);
        break;
    }

    CommonData::setcurrent_lastpressedphysicalkey(p->params);

    queue_->pop_front();

    setTimer();
  }
}
