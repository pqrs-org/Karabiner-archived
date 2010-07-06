#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "PressDownKeys.hpp"
#include "RemapClass.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Queue* EventOutputQueue::queue_ = NULL;
  IntervalChecker EventOutputQueue::ic_;
  TimerWrapper EventOutputQueue::timer_;

  void
  EventOutputQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new Queue();
    ic_.begin();
    timer_.initialize(&workloop, NULL, EventOutputQueue::fire);
  }

  void
  EventOutputQueue::terminate(void)
  {
    timer_.terminate();

    if (queue_) {
      for (;;) {
        EventOutputQueue::Item* p = static_cast<EventOutputQueue::Item*>(queue_->front());
        if (! p) break;

        queue_->pop();
        delete p;
      }
      delete queue_;
    }
  }

  // ----------------------------------------------------------------------
  void
  EventOutputQueue::push(Item& p)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    bool isempty = queue_->empty();
    queue_->push(&p);

    // ------------------------------------------------------------
    if (isempty && ic_.getmillisec() > DELAY) {
      fire_nolock();
    } else {
      IOLOG_DEVEL("Params_KeyboardEventCallBack::Queue enqueued ic_.getmillisec() = %d\n", ic_.getmillisec());
      timer_.setTimeoutMS(DELAY, false);
    }

    ic_.begin();
  }

  void
  EventOutputQueue::push(const Params_KeyboardEventCallBack& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_UpdateEventFlagsCallback& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_KeyboardSpecialEventCallback& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_RelativePointerEventCallback& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_ScrollWheelEventCallback& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  // ----------------------------------------------------------------------
  void
  EventOutputQueue::fire(OSObject* owner, IOTimerEventSource* sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    fire_nolock();
  }

  void
  EventOutputQueue::fire_nolock(void)
  {
    if (! queue_) return;

    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;

    // ----------------------------------------
    queue_->pop();

    switch ((p->params).type) {
      case ParamsUnion::KEYBOARD:
        ((p->params).params.params_KeyboardEventCallBack)->apply();
        break;
      case ParamsUnion::UPDATE_FLAGS:
        ((p->params).params.params_UpdateEventFlagsCallback)->apply();
        break;
      case ParamsUnion::KEYBOARD_SPECIAL:
        ((p->params).params.params_KeyboardSpecialEventCallback)->apply();
        break;
      case ParamsUnion::RELATIVE_POINTER:
        ((p->params).params.params_RelativePointerEventCallback)->apply();
        break;
      case ParamsUnion::SCROLL_POINTER:
        ((p->params).params.params_ScrollWheelEventCallback)->apply();
        break;
    }

    delete p;

    // ----------------------------------------
    if (! queue_->empty()) {
      timer_.setTimeoutMS(DELAY);
    }
  }

  // ======================================================================
  Flags EventOutputQueue::FireModifiers::lastFlags_(0);

  void
  EventOutputQueue::FireModifiers::fire(Flags toFlags, KeyboardType keyboardType)
  {
    // At first I handle KeyUp and handle KeyDown next.
    // We need to end KeyDown at Command+Space to Option_L+Shift_L.
    //
    // When Option_L+Shift_L has a meaning (switch input language at Windows),
    // it does not works well when the last is KeyUp of Command.

    // ------------------------------------------------------------
    // Handle KEYPAD first

    // KeyUp
    if (lastFlags_.isOn(ModifierFlag::KEYPAD) && ! toFlags.isOn(ModifierFlag::KEYPAD)) {
      lastFlags_.remove(ModifierFlag::KEYPAD);

      Params_UpdateEventFlagsCallback::auto_ptr ptr(Params_UpdateEventFlagsCallback::alloc(lastFlags_));
      if (ptr) {
        EventOutputQueue::push(*ptr);
      }
    }
    if (! lastFlags_.isOn(ModifierFlag::KEYPAD) && toFlags.isOn(ModifierFlag::KEYPAD)) {
      lastFlags_.add(ModifierFlag::KEYPAD);

      Params_UpdateEventFlagsCallback::auto_ptr ptr(Params_UpdateEventFlagsCallback::alloc(lastFlags_));
      if (ptr) {
        EventOutputQueue::push(*ptr);
      }
    }

    // ------------------------------------------------------------
    // KeyUp
    for (int i = 0;; ++i) {
      ModifierFlag flag = FlagStatus::getFlag(i);
      if (flag == ModifierFlag::NONE) break;
      if (flag == ModifierFlag::EXTRA1) continue;
      if (flag == ModifierFlag::EXTRA2) continue;
      if (flag == ModifierFlag::EXTRA3) continue;
      if (flag == ModifierFlag::EXTRA4) continue;
      if (flag == ModifierFlag::EXTRA5) continue;

      if (! lastFlags_.isOn(flag)) continue;
      if (toFlags.isOn(flag)) continue;

      lastFlags_.remove(flag);

      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false));
      if (! ptr) continue;
      EventOutputQueue::push(*ptr);
    }

    // KeyDown
    for (int i = 0;; ++i) {
      ModifierFlag flag = FlagStatus::getFlag(i);
      if (flag == ModifierFlag::NONE) break;
      if (flag == ModifierFlag::EXTRA1) continue;
      if (flag == ModifierFlag::EXTRA2) continue;
      if (flag == ModifierFlag::EXTRA3) continue;
      if (flag == ModifierFlag::EXTRA4) continue;
      if (flag == ModifierFlag::EXTRA5) continue;

      if (! toFlags.isOn(flag)) continue;
      if (lastFlags_.isOn(flag)) continue;

      lastFlags_.add(flag);

      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false));
      if (! ptr) continue;
      EventOutputQueue::push(*ptr);
    }
  }

  // ======================================================================
  void
  EventOutputQueue::FireKey::fire(const Params_KeyboardEventCallBack& params)
  {
    // ----------------------------------------
    // handle virtual keys
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params));
    if (! ptr) return;
    Params_KeyboardEventCallBack& p = *ptr;

    if (Handle_VK_LOCK::handle(p)) return;
    if (Handle_VK_STICKY::handle(p)) return;
    if (Handle_VK_LAZY::handle(p)) return;
    if (Handle_VK_CHANGE_INPUTMODE::handle(p)) return;
    if (Handle_VK_CONFIG::handle(p)) return;
    if (Handle_VK_JIS_TOGGLE_EISUU_KANA::handle(p)) return;
    if (handle_VK_JIS_BACKSLASH(p)) return;
    if (handle_VK_JIS_YEN(p)) return;
    if (Handle_VK_JIS_TEMPORARY::handle(p)) return;
    if (RemapClassManager::handlevirtualkey(p)) return;
    if (p.key == KeyCode::VK_MODIFIER_EXTRA1 ||
        p.key == KeyCode::VK_MODIFIER_EXTRA2 ||
        p.key == KeyCode::VK_MODIFIER_EXTRA3 ||
        p.key == KeyCode::VK_MODIFIER_EXTRA4 ||
        p.key == KeyCode::VK_MODIFIER_EXTRA5) return;

    // ------------------------------------------------------------
    p.key.reverseNormalizeKey(p.flags, p.eventType, p.keyboardType);
    p.flags.stripEXTRA();

    // skip no-outputable keycodes.
    // Note: check before FireModifiers to avoid meaningless modifier event.
    if (p.key == KeyCode::VK_NONE ||
        p.key == KeyCode::VK_PSEUDO_KEY) {
      return;
    }

    FireModifiers::fire(p.flags, p.keyboardType);

    if (p.eventType == EventType::DOWN || p.eventType == EventType::UP) {
      EventOutputQueue::push(p);

      if (p.eventType == EventType::DOWN) {
        PressDownKeys::add(p.key, p.keyboardType);
      } else {
        PressDownKeys::remove(p.key, p.keyboardType);
      }
    }
  }

  void
  EventOutputQueue::FireKey::fire_downup(Flags flags, KeyCode key, KeyboardType keyboardType)
  {
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, flags, key, keyboardType, false));
    if (! ptr) return;
    Params_KeyboardEventCallBack& params = *ptr;

    Flags f = key.getModifierFlag();

    if (f != ModifierFlag::NONE) {
      // We operate FlagStatus for the case "key == KeyCode::CAPSLOCK".
      FlagStatus::increase(f);
      params.flags.add(f);
      FireKey::fire(params);

      FlagStatus::decrease(f);
      params.flags = flags;
      FireKey::fire(params);

    } else {
      params.eventType = EventType::DOWN;
      FireKey::fire(params);
      params.eventType = EventType::UP;
      FireKey::fire(params);
    }
  }

  // ======================================================================
  void
  EventOutputQueue::FireConsumer::fire(const Params_KeyboardSpecialEventCallback& params)
  {
    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(params));
    if (! ptr) return;

    Params_KeyboardSpecialEventCallback& p = *ptr;

    p.flags.stripEXTRA();

    // skip no-outputable keycodes.
    // Note: check before FireModifiers to avoid meaningless modifier event.
    if (p.key == ConsumerKeyCode::VK_NONE ||
        p.key == ConsumerKeyCode::VK_PSEUDO_KEY) {
      return;
    }

    FireModifiers::fire();

    EventOutputQueue::push(p);
  }

  // ======================================================================
  Buttons EventOutputQueue::FireRelativePointer::lastButtons_(0);

  void
  EventOutputQueue::FireRelativePointer::fire(Buttons toButtons, int dx, int dy)
  {
    // We do not fire event if no need.
    if (dx == 0 && dy == 0 && toButtons == lastButtons_) return;

    Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(toButtons, dx, dy));
    if (! ptr) return;
    Params_RelativePointerEventCallback& params = *ptr;

    lastButtons_ = toButtons;
    FireModifiers::fire();
    EventOutputQueue::push(params);
  }

  // ======================================================================
  void
  EventOutputQueue::FireScrollWheel::fire(const Params_ScrollWheelEventCallback& params)
  {
    FireModifiers::fire();
    EventOutputQueue::push(params);
  }
}
