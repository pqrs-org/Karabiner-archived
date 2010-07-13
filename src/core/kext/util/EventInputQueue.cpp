#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventInputQueue.hpp"
#include "EventWatcher.hpp"
#include "FlagStatus.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "NumHeldDownKeys.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* EventInputQueue::queue_ = NULL;
  IntervalChecker EventInputQueue::ic_;
  TimerWrapper EventInputQueue::timer_;

  void
  EventInputQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new List();
    ic_.begin();
    timer_.initialize(&workloop, NULL, EventInputQueue::fire);
  }

  void
  EventInputQueue::terminate(void)
  {
    timer_.terminate();

    if (queue_) {
      delete queue_;
    }
  }

  uint32_t
  EventInputQueue::calcdelay(void)
  {
    // if no SimultaneousKeyPresses is enabled, fire immediately.
    if (! RemapClassManager::isEventInputQueueDelayEnabled()) {
      return IMMEDIATELY_DELAY;
    }

    uint32_t ms = ic_.getmillisec();
    uint32_t delay = config.get_simultaneouskeypresses_delay();
    if (delay > ms) delay = ms;  // min(ms, delay)
    if (delay < MIN_DELAY) delay = MIN_DELAY;  // max(MIN_DELAY, delay)
    ic_.begin();
    return delay;
  }

  void
  EventInputQueue::enqueue_(const Params_KeyboardEventCallBack& p)
  {
    if (! queue_) return;

    // Because we handle the key repeat ourself, drop the key repeat.
    if (p.repeat) return;

    // --------------------
    uint32_t delay = calcdelay();
    queue_->push_back(new Item(p, delay));
  }

  void
  EventInputQueue::enqueue_(const Params_KeyboardSpecialEventCallback& p)
  {
    if (! queue_) return;

    // Because we handle the key repeat ourself, drop the key repeat.
    if (p.repeat) return;

    // --------------------
    uint32_t delay = calcdelay();
    queue_->push_back(new Item(p, delay));
  }

  void
  EventInputQueue::enqueue_(const Params_RelativePointerEventCallback& p)
  {
    if (! queue_) return;

    // --------------------
    uint32_t delay = calcdelay();
    queue_->push_back(new Item(p, delay));
  }

  void
  EventInputQueue::enqueue_(const Params_ScrollWheelEventCallback& p)
  {
    if (! queue_) return;

    // --------------------
    uint32_t delay = calcdelay();
    queue_->push_back(new Item(p, delay));
  }

  void
  EventInputQueue::setTimer(void)
  {
    if (! queue_) return;

    Item* front = static_cast<Item*>(queue_->front());
    if (front) {
      timer_.setTimeoutMS(front->delayMS, false);
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
    IOLockWrapper::ScopedLock lk_eventlock(CommonData::getEventLock());
    IOLockWrapper::ScopedLock lk(timer_.getlock());

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
    {
      IOLockWrapper::ScopedLock lk_device(ListHookedKeyboard::instance().getListLock());

      IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
      if (! device) return;

      ListHookedKeyboard::Item* item = static_cast<ListHookedKeyboard::Item*>(ListHookedKeyboard::instance().get_nolock(device));
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
      if (item->isEqualVendorIDProductID(DeviceVendorID(0x046d), DeviceProductID(0xc515))) {
        if (params.key == KeyCode::CONTROL_L) return;
        if (params.key == KeyCode::SHIFT_L) return;
      }

      // ------------------------------------------------------------
      CommonData::setcurrent_ts(ts);
      CommonData::setcurrent_vendorIDproductID(item->getVendorID(), item->getProductID());
      CommonData::setcurrent_keyboardType(params.keyboardType);
    }

    // ------------------------------------------------------------
    // Because we handle the key repeat ourself, drop the key repeat by hardware.
    if (repeat) return;

    // ------------------------------------------------------------
    if (params.ex_iskeydown) {
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
    enqueue_(params);

    // remap keys
    RemapClassManager::remap_simultaneouskeypresses();

    setTimer();
  }

  void
  EventInputQueue::push_UpdateEventFlagsCallback(OSObject* target,
                                                 unsigned flags,
                                                 OSObject* sender,
                                                 void* refcon)
  {
    IOLockWrapper::ScopedLock lk_eventlock(CommonData::getEventLock());
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    // ------------------------------------------------------------
    Params_UpdateEventFlagsCallback::auto_ptr ptr(Params_UpdateEventFlagsCallback::alloc(flags));
    if (! ptr) return;
    Params_UpdateEventFlagsCallback& params = *ptr;

    // ------------------------------------------------------------
    // update device priority by calling ListHookedKeyboard::instance().get(kbd).
    {
      IOLockWrapper::ScopedLock lk_device(ListHookedKeyboard::instance().getListLock());

      IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
      if (! device) return;

      ListHookedKeyboard::Item* item = static_cast<ListHookedKeyboard::Item*>(ListHookedKeyboard::instance().get_nolock(device));
      if (! item) return;

      // ------------------------------------------------------------
      CommonData::setcurrent_vendorIDproductID(item->getVendorID(), item->getProductID());
    }

    params.log();
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
    IOLockWrapper::ScopedLock lk_eventlock(CommonData::getEventLock());
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    // ------------------------------------------------------------
    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(EventType(eventType), Flags(flags), ConsumerKeyCode(key),
                                                                                                 flavor, guid, repeat));
    if (! ptr) return;
    Params_KeyboardSpecialEventCallback& params = *ptr;

    // ------------------------------------------------------------
    {
      IOLockWrapper::ScopedLock lk_device(ListHookedKeyboard::instance().getListLock());

      IOHIKeyboard* device = OSDynamicCast(IOHIKeyboard, sender);
      if (! device) return;

      ListHookedConsumer::Item* item = static_cast<ListHookedConsumer::Item*>(ListHookedConsumer::instance().get_nolock(device));

      // ------------------------------------------------------------
      CommonData::setcurrent_ts(ts);
      CommonData::setcurrent_vendorIDproductID(item->getVendorID(), item->getProductID());
    }

    // ------------------------------------------------------------
    // Because we handle the key repeat ourself, drop the key repeat by hardware.
    if (repeat) return;

    // ------------------------------------------------------------
    if (params.ex_iskeydown) {
      CommonData::setcurrent_workspacedata();
    }

    // ------------------------------------------------------------
    // clear temporary_count_
    FlagStatus::set();

    // ------------------------------------------------------------
    enqueue_(params);

    // remap keys
    RemapClassManager::remap_simultaneouskeypresses();

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
    IOLockWrapper::ScopedLock lk_eventlock(CommonData::getEventLock());
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    // ------------------------------------------------------------
    Buttons buttons(buttons_raw);
    Buttons justPressed;
    Buttons justReleased;

    {
      IOLockWrapper::ScopedLock lk_device(ListHookedKeyboard::instance().getListLock());

      IOHIPointing* device = OSDynamicCast(IOHIPointing, sender);
      if (! device) return;

      ListHookedPointing::Item* item = static_cast<ListHookedPointing::Item*>(ListHookedPointing::instance().get_nolock(device));
      if (! item) return;

      // ------------------------------------------------------------
      CommonData::setcurrent_ts(ts);
      CommonData::setcurrent_vendorIDproductID(item->getVendorID(), item->getProductID());

      // ------------------------------------------------------------
      justPressed = buttons.justPressed(item->get_previousbuttons());
      justReleased = buttons.justReleased(item->get_previousbuttons());
      item->set_previousbuttons(buttons);
    }

    if (justPressed != Buttons(0)) {
      CommonData::setcurrent_workspacedata();
    }

    // ------------------------------------------------------------
    // clear temporary_count_
    if (! config.general_lazy_modifiers_with_mouse_event) {
      FlagStatus::set();
    }

    // ------------------------------------------------------------
    // divide an event into button and cursormove events.
    for (int i = 0; i < ButtonStatus::MAXNUM; ++i) {
      PointingButton btn(1 << i);
      if (justPressed.isOn(btn)) {
        Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, 0, 0, btn, true));
        if (! ptr) return;
        enqueue_(*ptr);
      }
      if (justReleased.isOn(btn)) {
        Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, 0, 0, btn, false));
        if (! ptr) return;
        enqueue_(*ptr);
      }
    }
    if (dx != 0 || dy != 0) {
      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, dx, dy, PointingButton::NONE, false));
      if (! ptr) return;
      enqueue_(*ptr);
    }

    // remap keys
    RemapClassManager::remap_simultaneouskeypresses();

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
    IOLockWrapper::ScopedLock lk_eventlock(CommonData::getEventLock());
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    // ------------------------------------------------------------
    Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(deltaAxis1, deltaAxis2, deltaAxis3,
                                                                                         fixedDelta1, fixedDelta2, fixedDelta3,
                                                                                         pointDelta1, pointDelta2, pointDelta3,
                                                                                         options));
    if (! ptr) return;
    Params_ScrollWheelEventCallback& params = *ptr;

    // ------------------------------------------------------------
    {
      IOLockWrapper::ScopedLock lk_device(ListHookedKeyboard::instance().getListLock());

      IOHIPointing* device = OSDynamicCast(IOHIPointing, sender);
      if (! device) return;

      ListHookedPointing::Item* item = static_cast<ListHookedPointing::Item*>(ListHookedPointing::instance().get_nolock(device));
      if (! item) return;

      // ------------------------------------------------------------
      CommonData::setcurrent_ts(ts);
      CommonData::setcurrent_vendorIDproductID(item->getVendorID(), item->getProductID());
    }

    // ------------------------------------------------------------
    // clear temporary_count_
    if (! config.general_lazy_modifiers_with_mouse_event) {
      FlagStatus::set();
    }

    // ------------------------------------------------------------
    enqueue_(params);

    // remap keys
    RemapClassManager::remap_simultaneouskeypresses();

    setTimer();
  }

  // ======================================================================
  void
  EventInputQueue::fire(OSObject* /*notuse_owner*/, IOTimerEventSource* /*notuse_sender*/)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    //IOLOG_DEVEL("EventInputQueue::fire queue_->size = %d\n", static_cast<int>(queue_->size()));

    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;

    if (! p->dropped) {
      switch (p->params.type) {
        case ParamsUnion::KEYBOARD:
        {
          Params_KeyboardEventCallBack* params = (p->params).params.params_KeyboardEventCallBack;
          if (params) {
            if (params->ex_iskeydown) {
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
            NumHeldDownKeys::set(params->ex_iskeydown ? 1 : -1);

            Core::remap_KeyboardEventCallback(*params);
          }
          break;
        }

        case ParamsUnion::KEYBOARD_SPECIAL:
        {
          Params_KeyboardSpecialEventCallback* params = (p->params).params.params_KeyboardSpecialEventCallback;
          if (params) {
            if (params->ex_iskeydown) {
              EventWatcher::on();
            }

            // ------------------------------------------------------------
            NumHeldDownKeys::set(params->ex_iskeydown ? 1 : -1);

            Core::remap_KeyboardSpecialEventCallback(*params);
          }
          break;
        }

        case ParamsUnion::RELATIVE_POINTER:
        {
          Params_RelativePointerEventCallback* params = (p->params).params.params_RelativePointerEventCallback;
          if (params) {
            // ------------------------------------------------------------
            // We set EventWatcher::on only when Buttons pressed.
            // It's cause a problem when you use the following settings. (Unexpected FN_Lock is fired).
            //   - FN+CursorMove to ScrollWheel
            //   - FN to FN (+ When you type FN only, send FN_Lock)
            //
            // But, if we call EventWatcher::on every CursorMove event, unexpected cancel occurs.
            // It's more terrible than above problem.
            // So, we keep to call EventWatcher::on only when Buttons pressed.
            if (params->ex_button != PointingButton::NONE) {
              EventWatcher::on();
            }

            // ------------------------------------------------------------
            if (params->ex_button != PointingButton::NONE) {
              NumHeldDownKeys::set(params->ex_isbuttondown ? 1 : -1);
            }

            Core::remap_RelativePointerEventCallback(*params);
          }
          break;
        }

        case ParamsUnion::SCROLL_POINTER:
        {
          Params_ScrollWheelEventCallback* params = (p->params).params.params_ScrollWheelEventCallback;
          if (params) {
            // EventWatcher::on is not necessary.

            Core::remap_ScrollWheelEventCallback(*params);
          }
          break;
        }

        default:
          IOLOG_ERROR("%s unkown type\n", __PRETTY_FUNCTION__);
          break;
      }
    }

    queue_->pop_front();

    setTimer();
  }
}
