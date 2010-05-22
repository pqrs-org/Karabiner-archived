#include "CommonData.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "KeyCode.hpp"
#include "RemapUtil.hpp"
#include "util/KeyboardRepeat.hpp"
#include "util/KeyEventInputQueue.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    Buttons remappedButtons;
  }

  bool
  RemapUtil::KeyToKey::remap(RemapParams& remapParams,
                             KeyCode fromKeyCode, Flags fromFlags,
                             KeyCode toKeyCode,   Flags toFlags,
                             bool isSetKeyRepeat)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    // ------------------------------------------------------------
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();
    if (isKeyDown) {
      if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
      active_ = true;

    } else {
      // When active_ is true, we converted the key at KeyDown.
      // So we also convert the key at KeyUp.
      //
      // We don't check the flags in KeyUp.
      // When we decide by flags, a problem occurs in the following situation.
      //
      // ex. "Shift+Delete to Forward Delete"
      // (1) KeyDown "Delete" => "Delete"
      // (2) KeyDown "Shift"  => "Shift"
      // (3) KeyUp   "Delete" => "Forward Delete" *** Bad ***
      // (4) KeyUp   "Shift"  => "Shift"

      if (! active_) return false;
      active_ = false;
    }

    // ------------------------------------------------------------
    remapParams.isremapped = true;

    // ------------------------------------------------------------
    // handle EventType & Modifiers
    EventType newEventType = remapParams.params.eventType;
    ModifierFlag fromModifierFlag = fromKeyCode.getModifierFlag();
    ModifierFlag toModifierFlag = toKeyCode.getModifierFlag();

    if (toModifierFlag == ModifierFlag::NONE) {
      if (fromModifierFlag == ModifierFlag::NONE) {
        // key2key

      } else {
        // modifier2key
        if (remapParams.params.flags.isOn(fromModifierFlag)) {
          FlagStatus::decrease(fromModifierFlag);
          newEventType = EventType::DOWN;
        } else {
          FlagStatus::increase(fromModifierFlag);
          newEventType = EventType::UP;
        }
      }

      FlagStatus::temporary_decrease(fromFlags);
      FlagStatus::temporary_increase(toFlags);

    } else {
      if (fromModifierFlag == ModifierFlag::NONE) {
        // key2modifier
        if (remapParams.params.eventType == EventType::DOWN) {
          FlagStatus::increase(toFlags | toModifierFlag);
          FlagStatus::decrease(fromFlags);
        } else if (remapParams.params.eventType == EventType::UP) {
          FlagStatus::decrease(toFlags | toModifierFlag);
          FlagStatus::increase(fromFlags);
        }
        newEventType = EventType::MODIFY;

      } else {
        // modifier2modifier
        if (remapParams.params.flags.isOn(fromModifierFlag)) {
          FlagStatus::increase(toFlags | toModifierFlag);
          FlagStatus::decrease(fromFlags | fromModifierFlag);
        } else {
          FlagStatus::decrease(toFlags | toModifierFlag);
          FlagStatus::increase(fromFlags | fromModifierFlag);
        }
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
    RemapUtil::fireKey(params);

    return true;
  }

  namespace {
    void
    firekeycombination(const RemapParams& remapParams, KeyCode key, Flags flags)
    {
      FlagStatus::temporary_increase(flags);

      Flags f = FlagStatus::makeFlags();
      KeyboardType keyboardType = remapParams.params.keyboardType;

      RemapUtil::fireKey_downup(f, key, keyboardType);
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
  RemapUtil::ConsumerToKey::remap(RemapConsumerParams& remapParams,
                                  ConsumerKeyCode fromKeyCode, Flags fromFlags,
                                  KeyCode toKeyCode,           Flags toFlags)
  {
    EventType eventType = remapParams.params.eventType;

    bool result = consumertoconsumer_.remap(remapParams, fromKeyCode, fromFlags, ConsumerKeyCode::VK_NONE);
    if (! result) return false;

    // ----------------------------------------
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(eventType,
                                                                                   FlagStatus::makeFlags(),
                                                                                   KeyCode::VK_CONSUMERKEY,
                                                                                   CommonData::getcurrent_keyboardType(),
                                                                                   remapParams.params.repeat));
    if (! ptr) return false;
    Params_KeyboardEventCallBack& params = *ptr;

    RemapParams rp(params);
    if (! keytokey_.remap(rp, KeyCode::VK_CONSUMERKEY, toKeyCode, toFlags)) {
      return false;
    }

    remapParams.drop();
    return true;
  }

  bool
  RemapUtil::ConsumerToConsumer::remap(RemapConsumerParams& remapParams,
                                       ConsumerKeyCode fromKeyCode, Flags fromFlags,
                                       ConsumerKeyCode toKeyCode,   Flags toFlags)
  {
    // ------------------------------------------------------------
    // NumLock Hack
    // If we change NumLock key, we need to call IOHIKeyboard::setNumLock(false).
    // Unless call setNumLock, internal NumLock status of IOHIKeyboard is still active.
    // And NumLock retains working status.
    if (fromKeyCode == ConsumerKeyCode::NUMLOCK && toKeyCode != ConsumerKeyCode::NUMLOCK) {
      HookedKeyboard* hk = ListHookedKeyboard::instance().get();
      if (hk) {
        IOHIKeyboard* kbd = hk->get();
        if (kbd) {
          if (kbd->numLock()) {
            kbd->setNumLock(false);
          }
        }
      }
    }

    // ------------------------------------------------------------
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    if (remapParams.params.eventType == EventType::DOWN) {
      // See RemapUtil::KeyToKey::remap about handling of "active_".
      if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
      active_ = true;

    } else {
      if (! active_) return false;
      active_ = false;
    }

    // ------------------------------------------------------------
    remapParams.isremapped = true;

    FlagStatus::temporary_decrease(fromFlags);
    FlagStatus::temporary_increase(toFlags);

    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(remapParams.params.eventType,
                                                                                                 FlagStatus::makeFlags(),
                                                                                                 toKeyCode,
                                                                                                 remapParams.params.repeat));
    if (! ptr) return false;
    Params_KeyboardSpecialEventCallback& params = *ptr;

    KeyboardRepeat::set(params);
    RemapUtil::fireConsumer(params);
    return true;
  }

  bool
  RemapUtil::KeyToConsumer::remap(RemapParams& remapParams,
                                  KeyCode fromKeyCode,       Flags fromFlags,
                                  ConsumerKeyCode toKeyCode, Flags toFlags)
  {
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();

    bool result = keytokey_.remap(remapParams, fromKeyCode, fromFlags, KeyCode::VK_NONE);
    if (! result) return false;

    EventType eventType = isKeyDown ? EventType::DOWN : EventType::UP;
    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(eventType,
                                                                                                 FlagStatus::makeFlags(),
                                                                                                 ConsumerKeyCode::VK_KEY,
                                                                                                 remapParams.params.repeat));
    if (! ptr) return false;
    Params_KeyboardSpecialEventCallback& params = *ptr;

    RemapConsumerParams rp(params);
    if (! consumertoconsumer_.remap(rp, ConsumerKeyCode::VK_KEY, toKeyCode, toFlags)) {
      return false;
    }

    remapParams.drop();
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

    if (remapParams.params.buttons.isOn(fromButton) &&
        FlagStatus::makeFlags().isOn(fromFlags)) {
      active_ = true;

    } else {
      if (active_) {
        // We handle it as a ButtonUp Event.
        active_ = false;
      } else {
        return false;
      }
    }

    // ----------------------------------------
    FlagStatus::temporary_decrease(fromFlags);
    FlagStatus::temporary_increase(toFlags);

    Params_RelativePointerEventCallback params = remapParams.params;

    if (active_) {
      params.buttons.remove(fromButton);
      params.buttons.add(toButton);

      remappedButtons.add(toButton);
    } else {
      remappedButtons.remove(toButton);
    }
    fireRelativePointer(params);

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
    }

    Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(PointingButton::VK_KEY, 0, 0));
    if (! ptr) return false;
    Params_RelativePointerEventCallback& params = *ptr;

    if (! isKeyDown) {
      params.buttons = PointingButton::NONE;
    }

    RemapPointingParams_relative rp(params);
    buttontobutton_.remap(rp, PointingButton::VK_KEY, toButton);

    if (! isKeyDown) {
      FlagStatus::increase(fromFlags | fromKeyCode.getModifierFlag());
      FireModifiers::fire();
    }

    return true;
  }

  // --------------------
  Flags FireModifiers::lastFlags_(0);

  void
  FireModifiers::fire(Flags toFlags, KeyboardType keyboardType)
  {
    // At first I handle KeyUp and handle KeyDown next.
    // We need to end KeyDown at Command+Space to Option_L+Shift_L.
    //
    // When Option_L+Shift_L has a meaning (switch input language at Windows),
    // it does not works well when the last is KeyUp of Command.

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
      Params_KeyboardEventCallBack& params = *ptr;
      params.apply();
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
      Params_KeyboardEventCallBack& params = *ptr;
      params.apply();
    }
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

    if (remapParams.params.buttons.isOn(buttons)) {
      // if the source buttons contains left button, we cancel left click for iPhoto, or some applications.
      // iPhoto store the scroll events when left button is pressed, and restore events after left button is released.
      // PointingRelativeToScroll doesn't aim it, we release the left button and do normal scroll event.
      if (buttons.isOn(PointingButton::LEFT)) {
        if (! active_) {
          Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(PointingButton::NONE, 0, 0));
          if (ptr) {
            RemapUtil::fireRelativePointer(*ptr);
          }
        }
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
    const int abs1 = (delta1 > 0 ? delta1 : -delta1);
    const int abs2 = (delta2 > 0 ? delta2 : -delta2);

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

    fireScrollWheel(params);
  }

  // ------------------------------------------------------------
  void
  RemapUtil::fireKey(const Params_KeyboardEventCallBack& params)
  {
    // ----------------------------------------
    // handle virtual keys
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params.eventType,
                                                                                   params.flags,
                                                                                   params.key,
                                                                                   params.charCode,
                                                                                   params.charSet,
                                                                                   params.origCharCode,
                                                                                   params.origCharSet,
                                                                                   params.keyboardType,
                                                                                   params.repeat));
    if (! ptr) return;
    Params_KeyboardEventCallBack& p = *ptr;

    if (Handle_VK_LOCK::handle(p)) return;
    if (Handle_VK_LAZY::handle(p)) return;
    if (Handle_VK_CHANGE_INPUTMODE::handle(p)) return;
    if (Handle_VK_CONFIG::handle(p)) return;
    if (Handle_VK_JIS_TOGGLE_EISUU_KANA::handle(p)) return;
    if (handle_VK_JIS_EISUU_x2(p)) return;
    if (handle_VK_JIS_KANA_x2(p)) return;
    if (handle_VK_JIS_BACKSLASH(p)) return;
    if (handle_VK_JIS_YEN(p)) return;
    if (Handle_VK_JIS_TEMPORARY::handle(p)) return;
    if (KeyEventInputQueue::handleVirtualKey(p)) return;
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
        p.key == KeyCode::VK_CONSUMERKEY) {
      return;
    }

    FireModifiers::fire(p.flags, p.keyboardType);

    if (p.eventType == EventType::DOWN || p.eventType == EventType::UP) {
      p.apply();

      if (p.eventType == EventType::DOWN) {
        PressDownKeys::add(p.key, p.keyboardType);
      } else {
        PressDownKeys::remove(p.key, p.keyboardType);
      }
    }
  }

  void
  RemapUtil::fireKey_downup(Flags flags, KeyCode key, KeyboardType keyboardType)
  {
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, flags, key, keyboardType, false));
    if (! ptr) return;
    Params_KeyboardEventCallBack& params = *ptr;

    Flags f = key.getModifierFlag();

    if (f != ModifierFlag::NONE) {
      // We operate FlagStatus for the case "key == KeyCode::CAPSLOCK".
      FlagStatus::increase(f);
      params.flags.add(f);
      RemapUtil::fireKey(params);

      FlagStatus::decrease(f);
      params.flags = flags;
      RemapUtil::fireKey(params);

    } else {
      params.eventType = EventType::DOWN;
      RemapUtil::fireKey(params);
      params.eventType = EventType::UP;
      RemapUtil::fireKey(params);
    }
  }

  void
  RemapUtil::fireConsumer(const Params_KeyboardSpecialEventCallback& params)
  {
    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(params.eventType,
                                                                                                 params.flags,
                                                                                                 params.key,
                                                                                                 params.flavor,
                                                                                                 params.guid,
                                                                                                 params.repeat));
    if (! ptr) return;

    Params_KeyboardSpecialEventCallback& p = *ptr;

    p.flags.stripEXTRA();

    // skip no-outputable keycodes.
    // Note: check before FireModifiers to avoid meaningless modifier event.
    if (p.key == ConsumerKeyCode::VK_NONE ||
        p.key == ConsumerKeyCode::VK_KEY) {
      return;
    }

    FireModifiers::fire();

    p.apply();
  }

  void
  RemapUtil::fireRelativePointer(const Params_RelativePointerEventCallback& params)
  {
    FireModifiers::fire();
    params.apply();
  }

  void
  RemapUtil::fireScrollWheel(const Params_ScrollWheelEventCallback& params)
  {
    FireModifiers::fire();
    params.apply();
  }

  // ----------------------------------------------------------------------
  bool
  KeyOverlaidModifier::remap(RemapParams& remapParams,
                             KeyCode fromKeyCode,   Flags fromFlags,
                             KeyCode toKeyCode,     Flags toFlags,
                             KeyCode fireKeyCode1,  Flags fireFlags1,
                             KeyCode fireKeyCode2,  Flags fireFlags2,
                             KeyCode fireKeyCode3,  Flags fireFlags3,
                             KeyCode fireKeyCode4,  Flags fireFlags4,
                             KeyCode fireKeyCode5,  Flags fireFlags5,
                             bool isFireRepeat)
  {
    // ----------------------------------------
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();
    bool savedIsAnyEventHappen = isAnyEventHappen_;

    if (! keytokey_.remap(remapParams, fromKeyCode, fromFlags, toKeyCode, toFlags)) {
      return false;
    }

    // ----------------------------------------
    if (isKeyDown) {
      EventWatcher::set(isAnyEventHappen_);
      ic_.begin();

      // ----------------------------------------
      // We store the flags when KeyDown.
      // Because it lets you make a natural input when the following sequence.
      //
      // ex. "Space to Shift (when type only, send Space)"
      // (1) KeyDown Command_L
      // (2) KeyDown Space        save flags (Command_L)
      // (3) KeyUp   Command_L
      // (4) KeyUp   Space        fire Command_L+Space

      // calc flags
      ModifierFlag toKeyCodeFlag = toKeyCode.getModifierFlag();
      FlagStatus::temporary_decrease(toFlags | toKeyCodeFlag);

      savedflags_ = FlagStatus::makeFlags();

      // restore flags
      FlagStatus::temporary_increase(toFlags | toKeyCodeFlag);

      // ----------------------------------------
      if (isFireRepeat) {
        KeyboardRepeat::cancel();

        KeyboardRepeat::primitive_add(EventType::DOWN, (fireFlags1 | savedflags_).stripNONE(), fireKeyCode1, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::UP,   (fireFlags1 | savedflags_).stripNONE(), fireKeyCode1, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::DOWN, (fireFlags2 | savedflags_).stripNONE(), fireKeyCode2, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::UP,   (fireFlags2 | savedflags_).stripNONE(), fireKeyCode2, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::DOWN, (fireFlags3 | savedflags_).stripNONE(), fireKeyCode3, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::UP,   (fireFlags3 | savedflags_).stripNONE(), fireKeyCode3, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::DOWN, (fireFlags4 | savedflags_).stripNONE(), fireKeyCode4, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::UP,   (fireFlags4 | savedflags_).stripNONE(), fireKeyCode4, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::DOWN, (fireFlags5 | savedflags_).stripNONE(), fireKeyCode5, remapParams.params.keyboardType);
        KeyboardRepeat::primitive_add(EventType::UP,   (fireFlags5 | savedflags_).stripNONE(), fireKeyCode5, remapParams.params.keyboardType);

        KeyboardRepeat::primitive_start(config.get_keyoverlaidmodifier_initial_wait());
      }

    } else {
      if (savedIsAnyEventHappen == false) {
        if (config.parameter_keyoverlaidmodifier_timeout <= 0 || ic_.checkThreshold(config.parameter_keyoverlaidmodifier_timeout) == false) {
          RemapUtil::fireKey_downup((fireFlags1 | savedflags_).stripNONE(), fireKeyCode1, remapParams.params.keyboardType);
          RemapUtil::fireKey_downup((fireFlags2 | savedflags_).stripNONE(), fireKeyCode2, remapParams.params.keyboardType);
          RemapUtil::fireKey_downup((fireFlags3 | savedflags_).stripNONE(), fireKeyCode3, remapParams.params.keyboardType);
          RemapUtil::fireKey_downup((fireFlags4 | savedflags_).stripNONE(), fireKeyCode4, remapParams.params.keyboardType);
          RemapUtil::fireKey_downup((fireFlags5 | savedflags_).stripNONE(), fireKeyCode5, remapParams.params.keyboardType);
        }
      }
      EventWatcher::unset(isAnyEventHappen_);
    }

    return true;
  }

  // ----------------------------------------------------------------------
  bool
  DoublePressModifier::remap(RemapParams& remapParams, KeyCode fromKeyCode, KeyCode toKeyCode, KeyCode fireKeyCode, Flags fireFlags)
  {
    if (remapParams.isremapped || remapParams.params.key != fromKeyCode) {
      pressCount_ = 0;
      return false;
    }

    // ----------------------------------------
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();

    keytokey_.remap(remapParams, fromKeyCode, toKeyCode);

    if (ic_.getmillisec() > static_cast<uint32_t>(config.parameter_doublepressmodifier_threshold)) {
      pressCount_ = 0;
    }
    ic_.begin();

    if (isKeyDown) {
      ++pressCount_;
    } else {
      if (pressCount_ >= 2) {
        pressCount_ = 0;
        Flags flags = (FlagStatus::makeFlags() | fireFlags).stripNONE();
        RemapUtil::fireKey_downup(flags, fireKeyCode, remapParams.params.keyboardType);
      }
    }

    return true;
  }

  // ----------------------------------------
  bool
  ModifierHoldingKeyToKey::remap(RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags, KeyCode toKeyCode)
  {
    if (remapParams.isremapped || remapParams.params.key != fromKeyCode) {
      goto nottargetkey;
    }

    if (! FlagStatus::makeFlags().isOn(fromFlags)) goto nottargetkey;
    if (! ic_.checkThreshold(config.parameter_modifierholdingkeytokey_wait)) goto nottargetkey;

    return keytokey_.remap(remapParams, fromKeyCode, fromFlags, toKeyCode);

  nottargetkey:
    ic_.begin();
    return false;
  }

  // ----------------------------------------
  bool
  IgnoreMultipleSameKeyPress::remap(RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags)
  {
    if (remapParams.isremapped || ! FlagStatus::makeFlags().isOn(fromFlags)) {
      lastkeycode_ = KeyCode::VK_NONE;
      return false;
    }

    if (remapParams.params.key == fromKeyCode &&
        fromKeyCode == lastkeycode_) {
      // disable event.
      remapParams.drop();
      return true;
    }

    // set lastkeycode_ if KeyUp.
    if (! remapParams.isKeyDownOrModifierDown()) {
      lastkeycode_ = remapParams.params.key;
    }
    return false;
  }

  // ------------------------------------------------------------
  Buttons
  RemapUtil::getRemappedButtons(void)
  {
    return remappedButtons;
  }
}
