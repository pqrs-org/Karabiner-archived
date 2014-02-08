#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "PressDownKeys.hpp"
#include "RemapClass.hpp"
#include "VirtualKey.hpp"
#include "VK_IOHIDPOSTEVENT.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* EventOutputQueue::queue_ = NULL;
  TimerWrapper EventOutputQueue::fire_timer_;

  void
  EventOutputQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new List();
    fire_timer_.initialize(&workloop, NULL, EventOutputQueue::fire_timer_callback);
  }

  void
  EventOutputQueue::terminate(void)
  {
    fire_timer_.terminate();

    if (queue_) {
      delete queue_;
    }
  }

  // ----------------------------------------------------------------------
#define PUSH_TO_OUTPUTQUEUE {               \
    if (! queue_) return;                   \
                                            \
    queue_->push_back(new Item(p));         \
    fire_timer_.setTimeoutMS(DELAY, false); \
}
  void EventOutputQueue::push(const Params_KeyboardEventCallBack& p) {
    PUSH_TO_OUTPUTQUEUE;
    if (p.eventType == EventType::DOWN) {
      FlagStatus::sticky_clear();
    }
  }
  void EventOutputQueue::push(const Params_UpdateEventFlagsCallback& p) {
    PUSH_TO_OUTPUTQUEUE;
  }
  void EventOutputQueue::push(const Params_KeyboardSpecialEventCallback& p) {
    PUSH_TO_OUTPUTQUEUE;
    if (p.ex_iskeydown) {
      FlagStatus::sticky_clear();
    }
  }
  void EventOutputQueue::push(const Params_RelativePointerEventCallback& p) {
    PUSH_TO_OUTPUTQUEUE;
    if (p.buttons != Buttons(0)) {
      FlagStatus::sticky_clear();
    }
  }
  void EventOutputQueue::push(const Params_ScrollWheelEventCallback& p) {
    PUSH_TO_OUTPUTQUEUE;
    FlagStatus::sticky_clear();
  }
  void EventOutputQueue::push(const Params_Wait& p) {
    PUSH_TO_OUTPUTQUEUE;
  }
#undef PUSH_TO_OUTPUTQUEUE

  // ----------------------------------------------------------------------
  void
  EventOutputQueue::fire_timer_callback(OSObject* /* owner */, IOTimerEventSource* /* sender */)
  {
    if (! queue_) return;

    // IOLOG_DEVEL("EventOutputQueue::fire queue_->size = %d\n", static_cast<int>(queue_->size()));

    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;

    int delay = DELAY;

    // ----------------------------------------
    switch ((p->params).type) {
      case ParamsUnion::KEYBOARD:
      {
        Params_KeyboardEventCallBack* params = (p->params).get_Params_KeyboardEventCallBack();
        if (params) {
          bool handled = VirtualKey::handleAfterEnqueued(*params);
          if (! handled) {
            ListHookedKeyboard::instance().apply(*params);
          }
          delay = Config::get_wait_between_sequential_keys();
        }
        break;
      }
      case ParamsUnion::UPDATE_FLAGS:
      {
        Params_UpdateEventFlagsCallback* params = (p->params).get_Params_UpdateEventFlagsCallback();
        if (params) {
          ListHookedKeyboard::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::KEYBOARD_SPECIAL:
      {
        Params_KeyboardSpecialEventCallback* params = (p->params).get_Params_KeyboardSpecialEventCallback();
        if (params) {
          if (! ListHookedConsumer::instance().apply(*params)) {
            // If there is no consumer device, we send an event as a software key.
            VirtualKey::VK_IOHIDPOSTEVENT::post(*params);
          }
        }
        break;
      }
      case ParamsUnion::RELATIVE_POINTER:
      {
        Params_RelativePointerEventCallback* params = (p->params).get_Params_RelativePointerEventCallback();
        if (params) {
          ListHookedPointing::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::SCROLL_WHEEL:
      {
        Params_ScrollWheelEventCallback* params = (p->params).get_Params_ScrollWheelEventCallback();
        if (params) {
          ListHookedPointing::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::WAIT:
      {
        Params_Wait* params = (p->params).get_Params_Wait();
        if (params) {
          delay = params->milliseconds;
        }
        break;
      }
    }

    queue_->pop_front();

    // ----------------------------------------
    if (! queue_->empty()) {
      fire_timer_.setTimeoutMS(delay);
    }
  }

  // ======================================================================
  Flags EventOutputQueue::FireModifiers::lastFlags_(0);

  void
  EventOutputQueue::FireModifiers::fire(Flags toFlags, KeyboardType keyboardType)
  {
    toFlags.stripNONE();
    toFlags.stripEXTRA();

    if (lastFlags_ == toFlags) return;

    // ------------------------------------------------------------
    // At first we handle KeyUp events and handle KeyDown events next.
    // We need to end KeyDown at Command+Space to Option_L+Shift_L.
    //
    // When Option_L+Shift_L has a meaning (switch input language at Windows),
    // it does not works well when the last is KeyUp of Command.

    // ------------------------------------------------------------
    // About ModifierFlag::CURSOR (UpdateEventFlags) handling:
    //
    // We need to treat ModifierFlag::CURSOR specially.
    //
    // When we activated "Control+Right to Option+Right" and pressed Control+Right,
    // the following events are happened.
    //
    // ----------------------+----------------------------------------------------------------------
    // (1) Press Control_L   | eventType:keyMod  code:0x3b name:Control_L flags:               misc:
    // (2) Press Right       | eventType:keyMod  code:0x3a name:Option_L  flags:Opt            misc:
    //                       | eventType:keyDown code:0x7c name:Right     flags:Opt NumPad Fn  misc:
    // (3) Release Right     | eventType:keyUp   code:0x7c name:Right     flags:Opt NumPad Fn  misc:
    // (4) Release Control_L | eventType:keyMod  code:0x3a name:Option_L  flags:               misc:
    // ----------------------+----------------------------------------------------------------------
    //
    // We need to treat "ModifierFlag::CURSOR Down" event after other modifiers.
    // We need to treat "ModifierFlag::CURSOR Up" event before other modifiers.
    // If not, unnecessary ModifierFlag::CURSOR is added on keyMod events.

    // ModifierFlag::CURSOR (Up)
    if (lastFlags_.isOn(ModifierFlag::CURSOR) && ! toFlags.isOn(ModifierFlag::CURSOR)) {
      lastFlags_.remove(ModifierFlag::CURSOR);

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
      if (flag == ModifierFlag::CURSOR) continue;
      if (Flags(flag).isVirtualModifiersOn()) continue;

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
      if (flag == ModifierFlag::CURSOR) continue;
      if (Flags(flag).isVirtualModifiersOn()) continue;

      if (! toFlags.isOn(flag)) continue;
      if (lastFlags_.isOn(flag)) continue;

      lastFlags_.add(flag);

      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false));
      if (! ptr) continue;
      EventOutputQueue::push(*ptr);
    }

    // ------------------------------------------------------------
    // ModifierFlag::CURSOR (Down)
    if (! lastFlags_.isOn(ModifierFlag::CURSOR) && toFlags.isOn(ModifierFlag::CURSOR)) {
      lastFlags_.add(ModifierFlag::CURSOR);

      Params_UpdateEventFlagsCallback::auto_ptr ptr(Params_UpdateEventFlagsCallback::alloc(lastFlags_));
      if (ptr) {
        EventOutputQueue::push(*ptr);
      }
    }

    lastFlags_ = toFlags;
  }

  // ======================================================================
  void
  EventOutputQueue::FireKey::fire(const Params_KeyboardEventCallBack& params)
  {
    if (VirtualKey::handle(params)) return;

    // ----------------------------------------
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
    ModifierFlag f = key.getModifierFlag();

    if (f != ModifierFlag::NONE) {
      FlagStatus::ScopedTemporaryFlagsChanger stfc(flags);

      // We operate FlagStatus for the case "key == KeyCode::CAPSLOCK".
      FlagStatus::increase(f);
      {
        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, FlagStatus::makeFlags(), key, keyboardType, false));
        if (! ptr) return;
        FireKey::fire(*ptr);
      }

      FlagStatus::decrease(f);
      {
        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, FlagStatus::makeFlags(), key, keyboardType, false));
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

  void
  EventOutputQueue::FireConsumer::fire_downup(Flags flags, ConsumerKeyCode key)
  {
    {
      Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(EventType::DOWN,
                                                                                                   flags,
                                                                                                   key,
                                                                                                   false));
      if (! ptr) return;
      FireConsumer::fire(*ptr);
    }
    {
      Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(EventType::UP,
                                                                                                   flags,
                                                                                                   key,
                                                                                                   false));
      if (! ptr) return;
      FireConsumer::fire(*ptr);
    }
  }

  // ======================================================================
  Buttons EventOutputQueue::FireRelativePointer::lastButtons_(0);

  void
  EventOutputQueue::FireRelativePointer::fire(Buttons toButtons, int dx, int dy)
  {
    // When changing space to command+left click,
    //   __KeyToKey__ KeyCode::SPACE, PointingButton::LEFT, ModifierFlag::COMMAND_L
    //
    // We need to release command key after left click was released as follows.
    //   (1) KeyDown   Command_L
    //   (2) MouseDown Left
    //   (3) MouseUp   Left
    //   (4) KeyUp     Command_L
    //
    // For mouse drag support, command modifier is increased as normal (not temporary_increase).
    // Therefore, command modifier is decreased when space key is released.
    // If we call FireModifiers::fire() at the begining of this function,
    // input events are fired as follows order.
    //   (1) KeyDown   Command_L
    //   (2) MouseDown Left
    //   (3) KeyUp     Command_L
    //   (4) MouseUp   Left
    //
    // It's not desired order.
    // So, we call FireModifiers::fire() at the end of this function when button is released.

    Buttons releasedButtons = lastButtons_;
    releasedButtons.remove(toButtons);
    bool isButtonReleased = ! releasedButtons.isNONE();

    if (! isButtonReleased) {
      FireModifiers::fire();
    }

    // Sending button event
    if (lastButtons_ != toButtons) {
      lastButtons_ = toButtons;

      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(toButtons, 0, 0, PointingButton::NONE, false));
      if (! ptr) return;
      Params_RelativePointerEventCallback& params = *ptr;

      EventOutputQueue::push(params);
    }

    // Sending cursor
    if (dx != 0 || dy != 0) {
      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(toButtons, dx, dy, PointingButton::NONE, false));
      if (! ptr) return;
      Params_RelativePointerEventCallback& params = *ptr;

      EventOutputQueue::push(params);
    }

    if (isButtonReleased) {
      FireModifiers::fire();
    }
  }

  // ======================================================================
  void
  EventOutputQueue::FireScrollWheel::fire(const Params_ScrollWheelEventCallback& params)
  {
    FireModifiers::fire();
    EventOutputQueue::push(params);
  }

  void
  EventOutputQueue::FireScrollWheel::fire(int delta1, int delta2)
  {
    short deltaAxis1;
    short deltaAxis2;
    IOFixed fixedDelta1;
    IOFixed fixedDelta2;
    SInt32 pointDelta1;
    SInt32 pointDelta2;

    int relative2scroll_rate = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_pointing_relative2scroll_rate);

    deltaAxis1 = (delta1 * relative2scroll_rate) / 1024 / DELTA_SCALE;
    deltaAxis2 = (delta2 * relative2scroll_rate) / 1024 / DELTA_SCALE;

    fixedDelta1 = (delta1 * relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024) / DELTA_SCALE;
    fixedDelta2 = (delta2 * relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024) / DELTA_SCALE;

    pointDelta1 = (delta1 * POINTING_POINT_SCALE * relative2scroll_rate) / 1024 / DELTA_SCALE;
    pointDelta2 = (delta2 * POINTING_POINT_SCALE * relative2scroll_rate) / 1024 / DELTA_SCALE;

    // see IOHIDSystem/IOHIDevicePrivateKeys.h about options.
    const int kScrollTypeContinuous_ = 0x0001;
    int options = kScrollTypeContinuous_;

    Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(deltaAxis1,  deltaAxis2, 0,
                                                                                         fixedDelta1, fixedDelta2, 0,
                                                                                         pointDelta1, pointDelta2, 0,
                                                                                         options));
    if (! ptr) return;
    EventOutputQueue::FireScrollWheel::fire(*ptr);
  }

  // ======================================================================
  void
  EventOutputQueue::FireWait::fire(const Params_Wait& params)
  {
    EventOutputQueue::push(params);
  }
}
