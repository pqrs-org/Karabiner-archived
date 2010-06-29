#include "CommonData.hpp"
#include "Config.hpp"
#include "EventOutput.hpp"
#include "FlagStatus.hpp"
#include "KeyCode.hpp"
#include "RemapClass.hpp"
#include "RemapUtil.hpp"
#include "util/KeyboardRepeat.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  RemapUtil::KeyToKey::remap(RemapParams& remapParams,
                             KeyCode fromKeyCode, Flags fromFlags,
                             KeyCode toKeyCode,   Flags toFlags,
                             bool isSetKeyRepeat)
  {
    if (remapParams.isremapped) return false;
    if (! fromkeychecker_.isFromKey(remapParams, fromKeyCode, fromFlags)) return false;
    remapParams.isremapped = true;

    // ------------------------------------------------------------
    // handle EventType & Modifiers
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();
    if (isKeyDown) {
      FlagStatus::decrease(fromKeyCode.getModifierFlag());
    } else {
      FlagStatus::increase(fromKeyCode.getModifierFlag());
    }

    // ----------------------------------------
    EventType newEventType = isKeyDown ? EventType::DOWN : EventType::UP;
    ModifierFlag toModifierFlag = toKeyCode.getModifierFlag();

    if (toModifierFlag == ModifierFlag::NONE) {
      // toKey
      FlagStatus::temporary_decrease(fromFlags);
      FlagStatus::temporary_increase(toFlags);

    } else {
      // toModifier
      newEventType = EventType::MODIFY;

      if (isKeyDown) {
        FlagStatus::increase(toFlags | toModifierFlag);
        FlagStatus::decrease(fromFlags);
      } else {
        FlagStatus::decrease(toFlags | toModifierFlag);
        FlagStatus::increase(fromFlags);
      }
    }

    // ----------------------------------------
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(newEventType,
                                                                                   FlagStatus::makeFlags(),
                                                                                   toKeyCode,
                                                                                   remapParams.params.keyboardType,
                                                                                   remapParams.params.repeat));
    if (! ptr) return false;
    Params_KeyboardEventCallBack& params = *ptr;

    if (isSetKeyRepeat) {
      KeyboardRepeat::set(params);
    }
    EventOutput::FireKey::fire(params);

    return true;
  }

  namespace {
    void
    firekeycombination(const RemapParams& remapParams, KeyCode key, Flags flags)
    {
      FlagStatus::temporary_increase(flags);

      Flags f = FlagStatus::makeFlags();
      KeyboardType keyboardType = remapParams.params.keyboardType;

      EventOutput::FireKey::fire_downup(f, key, keyboardType);
      KeyboardRepeat::primitive_add(EventType::DOWN, f, key, keyboardType);
      KeyboardRepeat::primitive_add(EventType::UP, f, key, keyboardType);

      FlagStatus::temporary_decrease(flags);
    }
  }

  bool
  RemapUtil::KeyToKey::remap(RemapParams& remapParams,
                             KeyCode fromKeyCode, Flags fromFlags,
                             KeyCode toKeyCode1,  Flags toFlags1,
                             KeyCode toKeyCode2,  Flags toFlags2,
                             KeyCode toKeyCode3,  Flags toFlags3,
                             KeyCode toKeyCode4,  Flags toFlags4,
                             KeyCode toKeyCode5,  Flags toFlags5)
  {
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();

    // We convert it into VK_NONE about the real hardware key.
    // This is necessary to output toKeyCode1 - toKeyCode5 at the KeyDown event.
    bool result = remap(remapParams, fromKeyCode, fromFlags, KeyCode::VK_NONE, ModifierFlag::NONE, false);
    if (! result) return false;

    KeyboardRepeat::cancel();

    if (isKeyDown) {
      firekeycombination(remapParams, toKeyCode1, toFlags1);
      firekeycombination(remapParams, toKeyCode2, toFlags2);
      firekeycombination(remapParams, toKeyCode3, toFlags3);
      firekeycombination(remapParams, toKeyCode4, toFlags4);
      firekeycombination(remapParams, toKeyCode5, toFlags5);

      KeyboardRepeat::primitive_start();
    }

    return true;
  }

  bool
  RemapUtil::PointingButtonToPointingButton::remap(RemapPointingParams_relative& remapParams,
                                                   PointingButton fromButton, Flags fromFlags,
                                                   PointingButton toButton,   Flags toFlags)
  {
    if (remapParams.isremapped) return false;

    // We consider it about Option_L+LeftClick to MiddleClick.
    // LeftClick generates the following events by ButtonDown and ButtonUp.
    //
    // (1) buttons == PointingButton::LEFT  (ButtonDown event)
    // (2) buttons == PointingButton::NONE  (ButtonUp event)
    //
    // We must cancel Option_L in both (1), (2).
    // We use "active_" flag to detect (2), because the "buttons" has no useful information at (2).
    //
    // Attention: We need fire MiddleClick only at (1).

    if (ButtonStatus::justPressed().isOn(fromButton) && FlagStatus::makeFlags().isOn(fromFlags)) {
      active_ = true;
      ButtonStatus::decrease(fromButton);
      ButtonStatus::increase(toButton);
    } else if (ButtonStatus::justReleased().isOn(fromButton) && active_) {
      // We need to handle ButtonUp Event, so keep active_.
      ButtonStatus::decrease(toButton);
      ButtonStatus::increase(fromButton);
    }

    if (! active_) return false;

    // ----------------------------------------
    FlagStatus::temporary_decrease(fromFlags);
    FlagStatus::temporary_increase(toFlags);

    EventOutput::FireRelativePointer::fire(ButtonStatus::makeButtons(), remapParams.params.dx, remapParams.params.dy);

    if (ButtonStatus::justReleased().isOn(fromButton)) {
      active_ = false;
    }

    remapParams.drop();
    return true;
  }

  bool
  RemapUtil::KeyToPointingButton::remap(RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags, PointingButton toButton)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    bool isKeyDown = remapParams.isKeyDownOrModifierDown();
    if (isKeyDown) {
      if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
      active_ = true;
    } else {
      if (! active_) return false;
      active_ = false;
    }

    // ------------------------------------------------------------
    remapParams.isremapped = true;

    // We need to call FlagStatus::decrease before mapping,
    // and call FlagStatus::increase after a mapping.
    //
    // ex. Option_L+Space to Right Button
    // (1) KeyDown Option_L
    //      1. KeyDown Option_L
    // (2) KeyDown Space
    //      2. KeyUp   Option_L
    //      3. ButtonDown Right
    // (3) KeyUp   Space
    //      4. ButtonUp   Right
    //      5. KeyUp   Option_L
    //
    // *** To support Mouse Dragging,
    // *** we need to use FlagStatus::decrease/increase.
    // *** (not temporary_decrease/temporary_increase).

    if (isKeyDown) {
      FlagStatus::decrease(fromFlags | fromKeyCode.getModifierFlag());
      ButtonStatus::increase(toButton);
      EventOutput::FireRelativePointer::fire();

    } else {
      ButtonStatus::decrease(toButton);
      EventOutput::FireRelativePointer::fire();

      FlagStatus::increase(fromFlags | fromKeyCode.getModifierFlag());
      EventOutput::FireModifiers::fire();
    }

    return true;
  }

  bool
  RemapUtil::PointingButtonToKey::remap(RemapPointingParams_relative& remapParams,
                                        PointingButton fromButton, Flags fromFlags,
                                        KeyCode toKeyCode1,  Flags toFlags1,
                                        KeyCode toKeyCode2,  Flags toFlags2,
                                        KeyCode toKeyCode3,  Flags toFlags3,
                                        KeyCode toKeyCode4,  Flags toFlags4,
                                        KeyCode toKeyCode5,  Flags toFlags5)
  {
    if (remapParams.isremapped) return false;

    if (ButtonStatus::justPressed().isOn(fromButton)) {
      if (FlagStatus::makeFlags().isOn(fromFlags)) {
        ButtonStatus::decrease(fromButton);
        active_ = true;
        goto doremap;
      }
    } else if (ButtonStatus::justReleased().isOn(fromButton)) {
      if (active_) {
        ButtonStatus::increase(fromButton);
        active_ = false;
        goto doremap;
      }
    }

    return false;

  doremap:
    // ----------------------------------------
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(active_ ? EventType::DOWN : EventType::UP,
                                                                                   FlagStatus::makeFlags(),
                                                                                   KeyCode::VK_PSEUDO_KEY,
                                                                                   CommonData::getcurrent_keyboardType(),
                                                                                   false));
    if (! ptr) return false;
    Params_KeyboardEventCallBack& params = *ptr;

    RemapParams rp(params);
    if (toKeyCode2 == KeyCode::VK_NONE) {
      if (! keytokey_.remap(rp, KeyCode::VK_PSEUDO_KEY, fromFlags, toKeyCode1, toFlags1)) {
        return false;
      }
    } else {
      if (! keytokey_.remap(rp, KeyCode::VK_PSEUDO_KEY, fromFlags,
                            toKeyCode1, toFlags1,
                            toKeyCode2, toFlags2,
                            toKeyCode3, toFlags3,
                            toKeyCode4, toFlags4,
                            toKeyCode5, toFlags5)) {
        return false;
      }
    }

    remapParams.drop();
    return true;
  }

  // ------------------------------------------------------------
  bool
  RemapUtil::PointingRelativeToScroll::remap(RemapPointingParams_relative& remapParams, Buttons buttons, Flags fromFlags)
  {
    if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;

    if (buttons == PointingButton::NONE) {
      FlagStatus::temporary_decrease(fromFlags);
      toscroll(remapParams);
      return true;
    }

    if (ButtonStatus::makeButtons().isOn(buttons)) {
      // if the source buttons contains left button, we cancel left click for iPhoto, or some applications.
      // iPhoto store the scroll events when left button is pressed, and restore events after left button is released.
      // PointingRelativeToScroll doesn't aim it, we release the left button and do normal scroll event.
      if (! active_) {
        ButtonStatus::decrease(buttons);
        EventOutput::FireRelativePointer::fire();
        ButtonStatus::increase(buttons);

        absolute_distance_ = 0;
        begin_ic_.begin();
        buffered_delta1 = 0;
        buffered_delta2 = 0;
      }

      active_ = true;
      FlagStatus::temporary_decrease(fromFlags);
      toscroll(remapParams);

      return true;

    } else {
      // ignore button up event.
      if (active_) {
        active_ = false;
        remapParams.isremapped = true;

        const uint32_t DISTANCE_THRESHOLD = 5;
        const uint32_t TIME_THRESHOLD = 300;
        if (absolute_distance_ <= DISTANCE_THRESHOLD && begin_ic_.getmillisec() < TIME_THRESHOLD) {
          // Fire by a click event.
          ButtonStatus::increase(buttons);
          EventOutput::FireRelativePointer::fire();
          ButtonStatus::decrease(buttons);
          EventOutput::FireRelativePointer::fire();
        }

        return true;
      }

      return false;
    }
  }

  void
  RemapUtil::PointingRelativeToScroll::toscroll(RemapPointingParams_relative& remapParams)
  {
    remapParams.isremapped = true;

    // ----------------------------------------
    // Buffer processing

    // buffer events in 20ms (60fps)
    const uint32_t BUFFER_MILLISEC = 20;

    buffered_delta1 += -remapParams.params.dy;
    buffered_delta2 += -remapParams.params.dx;

    if (buffered_ic_.getmillisec() < BUFFER_MILLISEC) {
      return;
    }

    int delta1 = buffered_delta1;
    int delta2 = buffered_delta2;
    buffered_delta1 = 0;
    buffered_delta2 = 0;
    buffered_ic_.begin();

    // ----------------------------------------
    if (config.option_pointing_disable_vertical_scroll) delta1 = 0;
    if (config.option_pointing_disable_horizontal_scroll) delta2 = 0;

    // ----------------------------------------
    // ignore minuscule move
    const unsigned int abs1 = RemapUtil::abs(delta1);
    const unsigned int abs2 = RemapUtil::abs(delta2);

    if (abs1 > abs2 * 2) {
      delta2 = 0;
    }
    if (abs2 > abs1 * 2) {
      delta1 = 0;
    }

    // ----------------------------------------
    // Fixation processing

    if (config.option_pointing_enable_scrollwheel_fixation) {
      // When 300ms passes from the last event, we reset a value.
      const uint32_t FIXATION_MILLISEC = 300;
      if (fixation_ic_.getmillisec() > FIXATION_MILLISEC) {
        fixation_begin_ic_.begin();
        fixation_delta1 = 0;
        fixation_delta2 = 0;
      }
      fixation_ic_.begin();

      if (fixation_delta1 > fixation_delta2 * 2) {
        delta2 = 0;
      }
      if (fixation_delta2 > fixation_delta1 * 2) {
        delta1 = 0;
      }

      // Only first 1000ms performs the addition of fixation_delta1, fixation_delta2.
      const uint32_t FIXATION_EARLY_MILLISEC  = 1000;
      if (fixation_begin_ic_.getmillisec() < FIXATION_EARLY_MILLISEC) {
        if (delta1 == 0) fixation_delta2 += abs2;
        if (delta2 == 0) fixation_delta1 += abs1;
      }
    }

    // ----------------------------------------
    if (delta1 == 0 && delta2 == 0) return;

    Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(0, 0, 0,
                                                                                         0, 0, 0,
                                                                                         0, 0, 0,
                                                                                         0));
    if (! ptr) return;
    Params_ScrollWheelEventCallback& params = *ptr;

    params.deltaAxis1 = (delta1 * config.pointing_relative2scroll_rate) / 1024;
    if (params.deltaAxis1 == 0 && delta1 != 0) {
      params.deltaAxis1 = delta1 > 0 ? 1 : -1;
    }
    params.deltaAxis2 = (delta2 * config.pointing_relative2scroll_rate) / 1024;
    if (params.deltaAxis2 == 0 && delta2 != 0) {
      params.deltaAxis2 = delta2 > 0 ? 1 : -1;
    }

    // ----------------------------------------
    params.fixedDelta1 = (delta1 * config.pointing_relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024);
    params.fixedDelta2 = (delta2 * config.pointing_relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024);

    params.pointDelta1 = (delta1 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1024;
    params.pointDelta2 = (delta2 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1024;

    EventOutput::FireScrollWheel::fire(params);

    absolute_distance_ += RemapUtil::abs(delta1) + RemapUtil::abs(delta2);
  }
}
