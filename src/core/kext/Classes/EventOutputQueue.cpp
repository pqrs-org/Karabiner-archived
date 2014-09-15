#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "PressDownKeys.hpp"
#include "RemapClass.hpp"
#include "VirtualKey.hpp"
#include "VK_IOHIDPOSTEVENT.hpp"

namespace org_pqrs_Karabiner {
  List EventOutputQueue::queue_;
  TimerWrapper EventOutputQueue::fire_timer_;
  Buttons EventOutputQueue::previousButtons_;

  void
  EventOutputQueue::initialize(IOWorkLoop& workloop)
  {
    fire_timer_.initialize(&workloop, NULL, EventOutputQueue::fire_timer_callback);
  }

  void
  EventOutputQueue::terminate(void)
  {
    fire_timer_.terminate();

    queue_.clear();
  }

  // ----------------------------------------------------------------------
#define PUSH_TO_OUTPUTQUEUE {           \
    queue_.push_back(new Item(p));      \
    fire_timer_.setTimeoutMS(0, false); \
}
  void EventOutputQueue::push(const Params_KeyboardEventCallBack& p) {
    PUSH_TO_OUTPUTQUEUE;
    if (p.eventType == EventType::DOWN) {
      FlagStatus::globalFlagStatus().sticky_clear();
    }
  }
  void EventOutputQueue::push(const Params_KeyboardSpecialEventCallback& p) {
    PUSH_TO_OUTPUTQUEUE;
    if (p.ex_iskeydown) {
      FlagStatus::globalFlagStatus().sticky_clear();
    }
  }
  void EventOutputQueue::push(const Params_RelativePointerEventCallback& p) {
    PUSH_TO_OUTPUTQUEUE;
    if (p.buttons != Buttons(0)) {
      FlagStatus::globalFlagStatus().sticky_clear();
    }
  }
  void EventOutputQueue::push(const Params_ScrollWheelEventCallback& p) {
    PUSH_TO_OUTPUTQUEUE;
    FlagStatus::globalFlagStatus().sticky_clear();
  }
  void EventOutputQueue::push(const Params_Wait& p) {
    PUSH_TO_OUTPUTQUEUE;
  }
#undef PUSH_TO_OUTPUTQUEUE

  // ----------------------------------------------------------------------
  unsigned int
  EventOutputQueue::calcDelay(const ParamsUnion& paramsUnion)
  {
    // ----------------------------------------
    {
      // Delay before and after modifier
      //
      // We need to wait before and after a modifier event because
      // events will be dropped by window server if
      // we send a mouse click event and a modifier event at the same time.
      //
      // For example, change middle click to command-left click by this autogen.
      // We need to put a wait on this setting in Mail.app.
      //    <autogen>
      //      __KeyToKey__
      //      PointingButton::MIDDLE, ModifierFlag::NONE,
      //      PointingButton::LEFT, ModifierFlag::COMMAND_L,
      //    </autogen>
      //
      if (paramsUnion.isModifier()) {
        return Config::get_wait_before_and_after_a_modifier_key_event();
      }
    }

    // ----------------------------------------
    {
      // Delay before and after modifier
      //
      // We need to wait before and after a pointing device click event because
      // events will be dropped by processes (eg. NetBeans) if
      // we send press button event and release button event at the same time.
      //

      auto params = paramsUnion.get_Params_RelativePointerEventCallback();
      if (params) {
        if (params->buttons != previousButtons_) {
          previousButtons_ = params->buttons;
          return Config::get_wait_before_and_after_a_click_event();
        }
      }
    }

    // ----------------------------------------
    return 0;
  }

  namespace {
    unsigned int maxDelay(unsigned int v1, unsigned int v2) {
      if (v1 > v2) {
        return v1;
      } else {
        return v2;
      }
    }
  }

  void
  EventOutputQueue::fire_timer_callback(OSObject* /* owner */, IOTimerEventSource* /* sender */)
  {
    // IOLOG_DEVEL("EventOutputQueue::fire queue_.size = %d\n", static_cast<int>(queue_.size()));

    Item* p = static_cast<Item*>(queue_.safe_front());
    if (! p) return;

    // Delay after modifier or click.
    unsigned int delay = calcDelay(p->params);

    // ----------------------------------------
    switch ((p->params).type) {
      case ParamsUnion::KEYBOARD:
      {
        auto params = (p->params).get_Params_KeyboardEventCallBack();
        if (params) {
          bool handled = VirtualKey::handleAfterEnqueued(*params);
          if (! handled) {
            ListHookedKeyboard::instance().apply(*params);
          }
        }

        // We need to wait at least 1ms in order to avoid changing key sequence order randomly.
        // (If VMware Fusion's driver is installed, the wrong order issue will be happen.)
        delay = maxDelay(delay, 1);
        break;
      }
      case ParamsUnion::KEYBOARD_SPECIAL:
      {
        auto params = (p->params).get_Params_KeyboardSpecialEventCallback();
        if (params) {
          if (! ListHookedConsumer::instance().apply(*params)) {
            // If there is no consumer device, we send an event as a software key.
            VirtualKey::VK_IOHIDPOSTEVENT::post(*params);
          }
        }

        // We need to wait at least 1ms in order to avoid changing key sequence order randomly.
        // (If VMware Fusion's driver is installed, the wrong order issue will be happen.)
        delay = maxDelay(delay, 1);
        break;
      }
      case ParamsUnion::RELATIVE_POINTER:
      {
        auto params = (p->params).get_Params_RelativePointerEventCallback();
        if (params) {
          ListHookedPointing::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::SCROLL_WHEEL:
      {
        auto params = (p->params).get_Params_ScrollWheelEventCallback();
        if (params) {
          ListHookedPointing::instance().apply(*params);
        }
        break;
      }
      case ParamsUnion::WAIT:
      {
        Params_Wait* params = (p->params).get_Params_Wait();
        if (params) {
          delay = maxDelay(delay, static_cast<unsigned int>(params->milliseconds));
        }
        break;
      }
      case ParamsUnion::UPDATE_FLAGS:
        // We need to wait at least 1ms in order to avoid changing key sequence order randomly.
        // (If VMware Fusion's driver is installed, the wrong order issue will be happen.)
        delay = maxDelay(delay, 1);
        break;
    }

    queue_.pop_front();

    // ----------------------------------------
    // Set timeout for next event.

    // Delay before modifier and click.
    Item* next = static_cast<Item*>(queue_.safe_front());
    if (! next) return;
    delay = maxDelay(delay, calcDelay(next->params));

    fire_timer_.setTimeoutMS(delay);
  }

  // ======================================================================
  Flags EventOutputQueue::FireModifiers::lastFlags_(0);

  void
  EventOutputQueue::FireModifiers::fire(Flags toFlags, KeyboardType keyboardType)
  {
    if (lastFlags_ == toFlags) return;

    // ------------------------------------------------------------
    // At first we handle KeyUp events and handle KeyDown events next.
    // We need to end KeyDown at Command+Space to Option_L+Shift_L.
    //
    // When Option_L+Shift_L has a meaning (switch input language at Windows),
    // it does not works well when the last is KeyUp of Command.

    // ------------------------------------------------------------
    // KeyUp
    for (size_t i = 0; i < FlagStatus::globalFlagStatus().itemSize(); ++i) {
      ModifierFlag flag = FlagStatus::globalFlagStatus().getFlag(i);

      // Skipping ModifierFlag::NUMPAD
      if (flag.getKeyCode() == KeyCode::VK_NONE) continue;
      // Skipping virtual modifiers.
      if (flag.getRawBits() == 0) continue;

      // ----------------------------------------
      if (! lastFlags_.isOn(flag)) continue;
      if (toFlags.isOn(flag)) continue;

      lastFlags_.remove(flag);

      Params_KeyboardEventCallBack params(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false);
      EventOutputQueue::push(params);
    }

    // KeyDown
    for (size_t i = 0; i < FlagStatus::globalFlagStatus().itemSize(); ++i) {
      ModifierFlag flag = FlagStatus::globalFlagStatus().getFlag(i);

      // Skipping ModifierFlag::NUMPAD
      if (flag.getKeyCode() == KeyCode::VK_NONE) continue;
      // Skipping virtual modifiers.
      if (flag.getRawBits() == 0) continue;

      // ----------------------------------------
      if (! toFlags.isOn(flag)) continue;
      if (lastFlags_.isOn(flag)) continue;

      lastFlags_.add(flag);

      Params_KeyboardEventCallBack params(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false);
      EventOutputQueue::push(params);
    }
  }

  // ======================================================================
  void
  EventOutputQueue::FireKey::fire(const Params_KeyboardEventCallBack& params)
  {
    if (VirtualKey::handle(params)) return;

    // ------------------------------------------------------------
    KeyCode newkeycode = params.key;
    Flags newflags = params.flags;

    KeyCode::reverseNormalizeKey(newkeycode, newflags, params.eventType, params.keyboardType);

    // skip no-outputable keycodes.
    // Note: check before FireModifiers to avoid meaningless modifier event.
    if (newkeycode == KeyCode::VK_NONE ||
        newkeycode == KeyCode::VK_PSEUDO_KEY) {
      return;
    }
    if (newkeycode.isModifier() && newkeycode.getModifierFlag().getRawBits() == 0) {
      // virtual modifiers.
      return;
    }

    FireModifiers::fire(newflags, params.keyboardType);

    if (params.eventType == EventType::DOWN || params.eventType == EventType::UP) {
      Params_KeyboardEventCallBack p(params.eventType, newflags, newkeycode,
                                     params.charCode, params.charSet, params.origCharCode, params.origCharSet,
                                     params.keyboardType, params.repeat);
      EventOutputQueue::push(p);

      if (! params.repeat) {
        if (params.eventType == EventType::DOWN) {
          PressDownKeys::add(newkeycode, params.keyboardType);
        } else {
          PressDownKeys::remove(newkeycode, params.keyboardType);
        }
      }
    }
  }

  // ======================================================================
  void
  EventOutputQueue::FireConsumer::fire(const Params_KeyboardSpecialEventCallback& params)
  {
    // skip no-outputable keycodes.
    // Note: check before FireModifiers to avoid meaningless modifier event.
    if (params.key == ConsumerKeyCode::VK_NONE ||
        params.key == ConsumerKeyCode::VK_PSEUDO_KEY) {
      return;
    }

    FireModifiers::fire();

    EventOutputQueue::push(params);
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

      Params_RelativePointerEventCallback params(toButtons, 0, 0, PointingButton::NONE, false);
      EventOutputQueue::push(params);
    }

    // Sending cursor
    if (dx != 0 || dy != 0) {
      Params_RelativePointerEventCallback params(toButtons, dx, dy, PointingButton::NONE, false);
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

    Params_ScrollWheelEventCallback params(deltaAxis1,  deltaAxis2, 0,
                                           fixedDelta1, fixedDelta2, 0,
                                           pointDelta1, pointDelta2, 0,
                                           options);
    EventOutputQueue::FireScrollWheel::fire(params);
  }

  // ======================================================================
  void
  EventOutputQueue::FireWait::fire(const Params_Wait& params)
  {
    EventOutputQueue::push(params);
  }
}
