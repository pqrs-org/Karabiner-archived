#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "PressDownKeys.hpp"
#include "RemapClass.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* EventOutputQueue::queue_ = NULL;
  TimerWrapper EventOutputQueue::timer_;

  void
  EventOutputQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new List();
    timer_.initialize(&workloop, NULL, EventOutputQueue::fire);
  }

  void
  EventOutputQueue::terminate(void)
  {
    timer_.terminate();

    if (queue_) {
      delete queue_;
    }
  }

  // ----------------------------------------------------------------------
#define PUSH_TO_OUTPUTQUEUE {                       \
    IOLockWrapper::ScopedLock lk(timer_.getlock()); \
                                                    \
    if (! queue_) return;                           \
                                                    \
    queue_->push_back(new Item(p));                 \
    timer_.setTimeoutMS(DELAY, false);              \
}
  void EventOutputQueue::push(const Params_KeyboardEventCallBack& p)        { PUSH_TO_OUTPUTQUEUE }
  void EventOutputQueue::push(const Params_UpdateEventFlagsCallback& p)     { PUSH_TO_OUTPUTQUEUE }
  void EventOutputQueue::push(const Params_KeyboardSpecialEventCallback& p) { PUSH_TO_OUTPUTQUEUE }
  void EventOutputQueue::push(const Params_RelativePointerEventCallback& p) { PUSH_TO_OUTPUTQUEUE }
  void EventOutputQueue::push(const Params_ScrollWheelEventCallback& p)     { PUSH_TO_OUTPUTQUEUE }
#undef PUSH_TO_OUTPUTQUEUE

  // ----------------------------------------------------------------------
  void
  EventOutputQueue::fire(OSObject* owner, IOTimerEventSource* sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    //IOLOG_DEVEL("EventOutputQueue::fire queue_->size = %d\n", static_cast<int>(queue_->size()));

    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;

    // ----------------------------------------
    switch ((p->params).type) {
      case ParamsUnion::KEYBOARD:
      {
        Params_KeyboardEventCallBack* params = (p->params).params.params_KeyboardEventCallBack;
        if (params) {
          ListHookedKeyboard::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::UPDATE_FLAGS:
      {
        Params_UpdateEventFlagsCallback* params = (p->params).params.params_UpdateEventFlagsCallback;
        if (params) {
          ListHookedKeyboard::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::KEYBOARD_SPECIAL:
      {
        Params_KeyboardSpecialEventCallback* params = (p->params).params.params_KeyboardSpecialEventCallback;
        if (params) {
          ListHookedConsumer::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::RELATIVE_POINTER:
      {
        Params_RelativePointerEventCallback* params = (p->params).params.params_RelativePointerEventCallback;
        if (params) {
          ListHookedPointing::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::SCROLL_POINTER:
      {
        Params_ScrollWheelEventCallback* params = (p->params).params.params_ScrollWheelEventCallback;
        if (params) {
          ListHookedPointing::instance().apply(*params);
        }
        break;
      }
    }

    queue_->pop_front();

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
    if (lastFlags_ == toFlags) return;

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
      if (Flags(flag).isVirtualModifiersOn()) continue;

      if (! lastFlags_.isOn(flag)) continue;
      if (toFlags.isOn(flag)) continue;

      // We consider the following case.
      //   - lastFlags_ = ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R
      //   - ModifierFlag::SHIFT_L is released
      //
      // *** NOTE ***
      // *** ModifierFlag::SHIFT_L = 0x20002 ***
      // *** ModifierFlag::SHIFT_R = 0x20004 ***
      //
      // Then, lastFlags_.remove(ModifierFlag::SHIFT_L) is invalid.
      // We must not drop 0x20000 from lastFlags_.
      // Do lastFlags_.remove(ModifierFlag::SHIFT_L).add(ModifierFlag::SHIFT_R)
      Flags tmp = lastFlags_;
      tmp.remove(flag);
      for (int j = 0;; ++j) {
        if (j == i) continue;

        ModifierFlag f = FlagStatus::getFlag(j);
        if (f == ModifierFlag::NONE) break;
        if (Flags(f).isVirtualModifiersOn()) continue;

        if (lastFlags_.isOn(f)) {
          tmp.add(f);
        }
      }
      lastFlags_ = tmp;

      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false));
      if (! ptr) continue;
      EventOutputQueue::push(*ptr);
    }

    // KeyDown
    for (int i = 0;; ++i) {
      ModifierFlag flag = FlagStatus::getFlag(i);
      if (flag == ModifierFlag::NONE) break;
      if (Flags(flag).isVirtualModifiersOn()) continue;

      if (! toFlags.isOn(flag)) continue;
      if (lastFlags_.isOn(flag)) continue;

      lastFlags_.add(flag);

      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false));
      if (! ptr) continue;
      EventOutputQueue::push(*ptr);
    }

    lastFlags_ = toFlags;
  }

  // ======================================================================
  void
  EventOutputQueue::FireKey::fire(const Params_KeyboardEventCallBack& params)
  {
    // ----------------------------------------
    // handle virtual keys
    if (Handle_VK_LOCK::handle(params)) return;
    if (Handle_VK_STICKY::handle(params)) return;
    if (Handle_VK_LAZY::handle(params)) return;
    if (Handle_VK_CHANGE_INPUTMODE::handle(params)) return;
    if (Handle_VK_CONFIG::handle(params)) return;
    if (Handle_VK_JIS_TOGGLE_EISUU_KANA::handle(params)) return;
    if (handle_VK_JIS_BACKSLASH(params)) return;
    if (handle_VK_JIS_YEN(params)) return;
    if (Handle_VK_JIS_TEMPORARY::handle(params)) return;
    if (params.key == KeyCode::VK_MODIFIER_EXTRA1 ||
        params.key == KeyCode::VK_MODIFIER_EXTRA2 ||
        params.key == KeyCode::VK_MODIFIER_EXTRA3 ||
        params.key == KeyCode::VK_MODIFIER_EXTRA4 ||
        params.key == KeyCode::VK_MODIFIER_EXTRA5) return;

    // ------------------------------------------------------------
    KeyCode newkeycode = params.key;
    Flags newflags = params.flags;

    KeyCode::reverseNormalizeKey(newkeycode, newflags, params.eventType, params.keyboardType);
    newflags.stripEXTRA();

    // skip no-outputable keycodes.
    // Note: check before FireModifiers to avoid meaningless modifier event.
    if (newkeycode == KeyCode::VK_NONE ||
        newkeycode == KeyCode::VK_PSEUDO_KEY) {
      return;
    }

    FireModifiers::fire(newflags, params.keyboardType);

    if (params.eventType == EventType::DOWN || params.eventType == EventType::UP) {
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params.eventType, newflags, newkeycode,
                                                                                     params.charCode, params.charSet, params.origCharCode, params.origCharSet,
                                                                                     params.keyboardType, params.repeat));
      if (ptr) {
        EventOutputQueue::push(*ptr);
      }

      if (! params.repeat) {
        if (params.eventType == EventType::DOWN) {
          PressDownKeys::add(newkeycode, params.keyboardType);
        } else {
          PressDownKeys::remove(newkeycode, params.keyboardType);
        }
      }
    }
  }

  void
  EventOutputQueue::FireKey::fire_downup(Flags flags, KeyCode key, KeyboardType keyboardType)
  {
    Flags f = key.getModifierFlag();

    if (f != ModifierFlag::NONE) {
      // We operate FlagStatus for the case "key == KeyCode::CAPSLOCK".
      FlagStatus::increase(f);
      {
        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, flags | f, key, keyboardType, false));
        if (! ptr) return;
        FireKey::fire(*ptr);
      }

      FlagStatus::decrease(f);
      {
        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, flags, key, keyboardType, false));
        if (! ptr) return;
        FireKey::fire(*ptr);
      }

    } else {
      {
        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::DOWN, flags, key, keyboardType, false));
        if (! ptr) return;
        FireKey::fire(*ptr);
      }
      {
        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::UP, flags, key, keyboardType, false));
        if (! ptr) return;
        FireKey::fire(*ptr);
      }
    }
  }

  // ======================================================================
  void
  EventOutputQueue::FireConsumer::fire(const Params_KeyboardSpecialEventCallback& params)
  {
    Flags newflags = params.flags;
    newflags.stripEXTRA();

    // skip no-outputable keycodes.
    // Note: check before FireModifiers to avoid meaningless modifier event.
    if (params.key == ConsumerKeyCode::VK_NONE ||
        params.key == ConsumerKeyCode::VK_PSEUDO_KEY) {
      return;
    }

    FireModifiers::fire();

    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(params.eventType, newflags, params.key, params.repeat));
    if (ptr) {
      EventOutputQueue::push(*ptr);
    }
  }

  // ======================================================================
  void
  EventOutputQueue::FireRelativePointer::fire(Buttons toButtons, int dx, int dy)
  {
    Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(toButtons, dx, dy, PointingButton::NONE, false));
    if (! ptr) return;
    Params_RelativePointerEventCallback& params = *ptr;

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
