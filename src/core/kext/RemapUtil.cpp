#include "CommonData.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "KeyCode.hpp"
#include "RemapUtil.hpp"
#include "util/KeyboardRepeat.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    Buttons remappedButtions;
  }

  bool
  RemapUtil::KeyToKey::remap(RemapParams& remapParams,
                             const KeyCode& fromKeyCode, const Flags& fromFlags,
                             const KeyCode& toKeyCode,   const Flags& toFlags,
                             bool isSetKeyRepeat)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    // ------------------------------------------------------------
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();
    if (isKeyDown) {
      // We consider a case of the key repeat for ConsumerToKey.
      // We continue remapping the key if it was remapped once.
      if (active_ == false) {
        if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
        active_ = true;
      }

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

    // We ignore the key repeat because we handle it by myself.
    //
    // The key repeat does not come to here by the handling of normal KeyToKey.
    // Because the key repeat is ignored in remap_KeyboardEventCallback.
    //
    // This processing is sake of ConsumerToKey.
    if (remapParams.params.repeat) {
      return true;
    }

    // ------------------------------------------------------------
    // handle EventType & Modifiers
    EventType newEventType = remapParams.params.eventType;
    const ModifierFlag& fromModifierFlag = fromKeyCode.getModifierFlag();
    const ModifierFlag& toModifierFlag = toKeyCode.getModifierFlag();

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
    Params_KeyboardEventCallBack params(newEventType,
                                        FlagStatus::makeFlags(),
                                        toKeyCode,
                                        remapParams.params.keyboardType,
                                        remapParams.params.repeat);
    RemapUtil::fireKey(params, remapParams.workspacedata);

    // ----------------------------------------
    if (isSetKeyRepeat) {
      KeyboardRepeat::set(remapParams.params);
    }

    return true;
  }

  bool
  RemapUtil::KeyToKey::remap(RemapParams& remapParams,
                             const KeyCode& fromKeyCode, const Flags& fromFlags,
                             const KeyCode& toKeyCode1,  const Flags& toFlags1,
                             const KeyCode& toKeyCode2,  const Flags& toFlags2)
  {
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();

    bool result = remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, false);
    if (! result) return false;

    if (isKeyDown) {
      // calc flags
      FlagStatus::temporary_decrease(toFlags1);
      FlagStatus::temporary_increase(toFlags2);

      Flags flags = FlagStatus::makeFlags();
      RemapUtil::fireKey_downup(flags, toKeyCode2, remapParams.params.keyboardType, remapParams.workspacedata);

      // restore flags
      FlagStatus::temporary_increase(toFlags1);
      FlagStatus::temporary_decrease(toFlags2);
    }

    return true;
  }

  // ----------
  bool
  RemapUtil::keyToPointingButton(RemapParams& remapParams, const KeyCode& fromKeyCode, const PointingButton& toButton)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    // XXX: use PointingButtonToPointingButton

    // ------------------------------------------------------------
    if (remapParams.isKeyDownOrModifierDown()) {
      FlagStatus::decrease(fromKeyCode.getModifierFlag());
      RemapUtil::fireRelativePointer(Params_RelativePointerEventCallback(toButton, 0, 0));
      remappedButtions.add(toButton);

    } else {
      FlagStatus::increase(fromKeyCode.getModifierFlag());
      RemapUtil::fireRelativePointer(Params_RelativePointerEventCallback(PointingButton::NONE, 0, 0));
      remappedButtions.remove(toButton);
    }

    remapParams.drop();
    return true;
  }

  bool
  RemapUtil::ConsumerToKey::remap(const RemapConsumerParams& remapParams,
                                  const ConsumerKeyCode& fromKeyCode, const Flags& fromFlags,
                                  const KeyCode& toKeyCode, const Flags& toFlags)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    // ----------------------------------------
    Params_KeyboardEventCallBack params(remapParams.params.eventType,
                                        FlagStatus::makeFlags(),
                                        KeyCode::VK_CONSUMERKEY,
                                        CommonData::getcurrent_keyboardType(),
                                        remapParams.params.repeat);
    RemapParams rp(params, remapParams.workspacedata);
    if (! keytokey_.remap(rp, KeyCode::VK_CONSUMERKEY, fromFlags, toKeyCode, toFlags)) {
      return false;
    }

    remapParams.drop();
    return true;
  }

  bool
  RemapUtil::ConsumerToConsumer::remap(const RemapConsumerParams& remapParams,
                                       const ConsumerKeyCode& fromKeyCode, const Flags& fromFlags,
                                       const ConsumerKeyCode& toKeyCode,   const Flags& toFlags)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    if (remapParams.params.eventType == EventType::DOWN) {
      // See RemapUtil::KeyToKey::remap about handling of "active_".
      if (active_ == false) {
        if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
        active_ = true;
      }

    } else {
      if (! active_) return false;
      active_ = false;
    }

    // ------------------------------------------------------------
    remapParams.isremapped = true;

    FlagStatus::temporary_decrease(fromFlags);
    FlagStatus::temporary_increase(toFlags);

    Params_KeyboardSpecialEventCallback params(remapParams.params.eventType,
                                               FlagStatus::makeFlags(),
                                               toKeyCode,
                                               remapParams.params.repeat);
    RemapUtil::fireConsumer(params);
    return true;
  }

  bool
  RemapUtil::KeyToConsumer::remap(RemapParams& remapParams,
                                  const KeyCode& fromKeyCode, const Flags& fromFlags,
                                  const ConsumerKeyCode& toKeyCode, const Flags& toFlags)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    // ----------------------------------------
    Params_KeyboardSpecialEventCallback params(remapParams.params.eventType,
                                               FlagStatus::makeFlags(),
                                               ConsumerKeyCode::VK_KEY,
                                               remapParams.params.repeat);

    bool isremapped = false;
    RemapConsumerParams rp = {
      params,
      remapParams.workspacedata,
      isremapped,
    };
    if (! consumertoconsumer_.remap(rp, ConsumerKeyCode::VK_KEY, fromFlags, toKeyCode, toFlags)) {
      return false;
    }

    remapParams.drop();
    return true;
  }

  bool
  RemapUtil::PointingButtonToPointingButton::remap(RemapPointingParams_relative& remapParams,
                                                   const PointingButton& fromButton, const Flags& fromFlags,
                                                   const PointingButton& toButton)
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

    Params_RelativePointerEventCallback params = remapParams.params;

    if (active_) {
      params.buttons.remove(fromButton);
      params.buttons.add(toButton);
    }
    fireRelativePointer(params);

    remapParams.drop();
    return true;
  }

  // --------------------
  Flags FireModifiers::lastFlags_;

  void
  FireModifiers::fire(const Flags& toFlags, const KeyboardType& keyboardType)
  {
    if (lastFlags_ == toFlags) return;
#if 0
    printf("FireModifiers::fire from:%x to:%x\n", lastFlags_.get(), toFlags.get());
#endif

    // ----------------------------------------------------------------------
    bool modifierStatus[FlagStatus::MAXNUM];

    // setup modifierStatus
    for (int i = 0; ; ++i) {
      const ModifierFlag& m = FlagStatus::getFlag(i);
      if (m == ModifierFlag::NONE) break;
      modifierStatus[i] = lastFlags_.isOn(m);
    }

    Params_KeyboardEventCallBack params(EventType::MODIFY, 0, KeyCode::VK_NONE,
                                        keyboardType, false);

    // ----------------------------------------------------------------------
    // fire

    // At first I handle KeyUp and handle KeyDown next.
    // We need to end KeyDown at Command+Space to Option_L+Shift_L.
    //
    // When Option_L+Shift_L has a meaning (switch input language at Windows),
    // it does not works well when the last is KeyUp of Command.

    bool listIsFireKeyUp[] = { true, false };
    for (size_t firetype = 0; firetype < sizeof(listIsFireKeyUp) / sizeof(listIsFireKeyUp[0]); ++firetype) {
      bool isFireKeyUp = listIsFireKeyUp[firetype];

      for (int i = 0; ; ++i) {
        const ModifierFlag& m = FlagStatus::getFlag(i);
        if (m == ModifierFlag::NONE) break;

        bool from = lastFlags_.isOn(m);
        bool to = toFlags.isOn(m);

        if (isFireKeyUp) {
          // fire Up only
          if (! (from == true && to == false)) continue;
        } else {
          // fire Down only
          if (! (from == false && to == true)) continue;
        }

        if (from) {
          modifierStatus[i] = false;
        } else {
          modifierStatus[i] = true;
        }

        Flags flags = 0;
        for (int j = 0; ; ++j) {
          const ModifierFlag& mm = FlagStatus::getFlag(j);
          if (mm == ModifierFlag::NONE) break;

          if (modifierStatus[j]) {
            flags.add(mm);
          }
        }

        params.flags = flags;
        params.key = m.getKeyCode();
        params.apply();
      }
    }

    lastFlags_ = toFlags;
  }

  // ------------------------------------------------------------
  namespace {
    class Handle_VK_FNLOCK {
    public:
      static bool handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
        if (params.key != KeyCode::VK_FNLOCK) return false;

        if (params.eventType == EventType::DOWN) {
          active_ = ! active_;
          if (active_) {
            FlagStatus::lock_increase(ModifierFlag::FN);
          } else {
            FlagStatus::lock_decrease(ModifierFlag::FN);
          }
        }

        return true;
      }

    private:
      static bool active_;
    };
    bool Handle_VK_FNLOCK::active_ = false;

    class Handle_VK_JIS_TOGGLE_EISUU_KANA {
    public:
      static bool handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
        if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

        if (params.eventType == EventType::DOWN) {
          if (workspacedata.inputmode == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE) {
            newkeycode_ = KeyCode::JIS_EISUU;
          } else {
            newkeycode_ = KeyCode::JIS_KANA;
          }
        }

        params.key = newkeycode_;
        RemapUtil::fireKey(params, workspacedata);
        return true;
      }

    private:
      // It is necessary to save toKeyCode for KeyUp.
      static KeyCode newkeycode_;
    };
    KeyCode Handle_VK_JIS_TOGGLE_EISUU_KANA::newkeycode_ = KeyCode::VK_NONE;

    bool
    handle_VK_JIS_EISUU_x2(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (params.key != KeyCode::VK_JIS_EISUU_x2) return false;

      if (params.eventType == EventType::DOWN) {
        for (int i = 0; i < 2; ++i) {
          RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_EISUU, params.keyboardType, workspacedata);
        }
      }
      return true;
    }

    bool
    handle_VK_JIS_KANA_x2(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (params.key != KeyCode::VK_JIS_KANA_x2) return false;

      if (params.eventType == EventType::DOWN) {
        for (int i = 0; i < 2; ++i) {
          RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_KANA, params.keyboardType, workspacedata);
        }
      }
      return true;
    }

    bool
    handle_VK_JIS_BACKSLASH(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (params.key != KeyCode::VK_JIS_BACKSLASH) return false;

      params.key = KeyCode::BACKSLASH;
      params.keyboardType = KeyboardType::MACBOOK;
      RemapUtil::fireKey(params, workspacedata);
      return true;
    }
  }

  void
  RemapUtil::fireKey(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    // ----------------------------------------
    // handle virtual keys
    Params_KeyboardEventCallBack p = params;
    if (Handle_VK_FNLOCK::handle(p, workspacedata)) return;
    if (Handle_VK_JIS_TOGGLE_EISUU_KANA::handle(p, workspacedata)) return;
    if (handle_VK_JIS_EISUU_x2(p, workspacedata)) return;
    if (handle_VK_JIS_KANA_x2(p, workspacedata)) return;
    if (handle_VK_JIS_BACKSLASH(p, workspacedata)) return;

    // ------------------------------------------------------------
    p.key.reverseNormalizeKey(p.flags, p.keyboardType);

    FireModifiers::fire(p.flags, p.keyboardType);

    // skip no-outputable keycodes.
    if (p.key == KeyCode::VK_NONE ||
        p.key == KeyCode::VK_CONSUMERKEY) {
      return;
    }

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
  RemapUtil::fireKey_downup(const Flags& flags, const KeyCode& key, const KeyboardType& keyboardType,
                            const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    Params_KeyboardEventCallBack params(EventType::MODIFY, flags, key, keyboardType, false);

    if (key.isModifier()) {
      RemapUtil::fireKey(params, workspacedata);
    } else {
      params.eventType = EventType::DOWN;
      RemapUtil::fireKey(params, workspacedata);
      params.eventType = EventType::UP;
      RemapUtil::fireKey(params, workspacedata);
    }
  }

  void
  RemapUtil::fireConsumer(const Params_KeyboardSpecialEventCallback& params)
  {
    FireModifiers::fire();
    params.apply();
  }

  void
  RemapUtil::fireRelativePointer(const Params_RelativePointerEventCallback& params)
  {
    FireModifiers::fire();
    params.apply();
  }

  void
  RemapUtil::fireScrollWheel(short int deltaAxis1, short int deltaAxis2, short int deltaAxis3,
                             IOFixed fixedDelta1, IOFixed fixedDelta2, IOFixed fixedDelta3,
                             SInt32 pointDelta1, SInt32 pointDelta2, SInt32 pointDelta3)
  {
    Params_ScrollWheelEventCallback params(deltaAxis1,  deltaAxis2,  deltaAxis3,
                                           fixedDelta1, fixedDelta2, fixedDelta3,
                                           pointDelta1, pointDelta2, pointDelta3,
                                           0);
    params.apply();
  }

  // --------------------
  void
  RemapUtil::pointingRelativeToScroll(RemapPointingParams_relative& remapParams)
  {
    remapParams.isremapped = true;

    // ----------------------------------------
    int delta1 = -remapParams.params.dy;
    int delta2 = -remapParams.params.dx;

    if (config.option_pointing_disable_vertical_scroll) delta1 = 0;
    if (config.option_pointing_disable_horizontal_scroll) delta2 = 0;

    // ----------------------------------------
    // ignore minuscule move
    int abs1 = delta1 > 0 ? delta1 : -delta1;
    int abs2 = delta2 > 0 ? delta2 : -delta2;
    int SCALE = 100;

    if (abs1 > abs2) {
      // case y > x (ignore x if x is very small)
      if (abs2) {
        if ((abs1 * SCALE / abs2) > (10 * SCALE / 5)) delta2 = 0;
      }
    }
    if (abs1 < abs2) {
      if (abs1) {
        if ((abs2 * SCALE / abs1) > (10 * SCALE / 5)) delta1 = 0;
      }
    }

    if (delta1 == 0 && delta2 == 0) return;

    // ----------------------------------------
    int deltaAxis1 = (delta1 * config.pointing_relative2scroll_rate) / 1000;
    if (deltaAxis1 == 0 && delta1 != 0) {
      deltaAxis1 = delta1 > 0 ? 1 : -1;
    }
    int deltaAxis2 = (delta2 * config.pointing_relative2scroll_rate) / 1000;
    if (deltaAxis2 == 0 && delta2 != 0) {
      deltaAxis2 = delta2 > 0 ? 1 : -1;
    }

    // ----------------------------------------
    IOFixed fixedDelta1 = (delta1 * POINTING_FIXED_SCALE * config.pointing_relative2scroll_rate) / 1000;
    IOFixed fixedDelta2 = (delta2 * POINTING_FIXED_SCALE * config.pointing_relative2scroll_rate) / 1000;

    SInt32 pointDelta1 = (delta1 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1000;
    SInt32 pointDelta2 = (delta2 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1000;

    fireScrollWheel(deltaAxis1, deltaAxis2, 0,
                    fixedDelta1, fixedDelta2, 0,
                    pointDelta1, pointDelta2, 0);
  }

  // ----------------------------------------------------------------------
  bool
  KeyOverlaidModifier::remap(RemapParams& remapParams,
                             const KeyCode& fromKeyCode, const Flags& fromFlags,
                             const KeyCode& toKeyCode,   const Flags& toFlags,
                             const KeyCode& fireKeyCode, const Flags& fireFlags,
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

      // calc flags
      ModifierFlag toKeyCodeFlag = toKeyCode.getModifierFlag();
      FlagStatus::temporary_decrease(toFlags | toKeyCodeFlag);
      FlagStatus::temporary_increase(fireFlags);

      savedflags_ = FlagStatus::makeFlags().get();

      // restore flags
      FlagStatus::temporary_increase(toFlags | toKeyCodeFlag);
      FlagStatus::temporary_decrease(fireFlags);

      if (isFireRepeat) {
        KeyboardRepeat::set(EventType::DOWN, savedflags_, fireKeyCode, remapParams.params.keyboardType,
                            config.get_keyoverlaidmodifier_initial_wait());
      }

    } else {
      if (savedIsAnyEventHappen == false) {
        if (config.parameter_keyoverlaidmodifier_timeout <= 0 || ic_.checkThreshold(config.parameter_keyoverlaidmodifier_timeout) == false) {
          RemapUtil::fireKey_downup(savedflags_, fireKeyCode, remapParams.params.keyboardType, remapParams.workspacedata);
        }
      }
      EventWatcher::unset(isAnyEventHappen_);
    }

    return true;
  }

  // ----------------------------------------------------------------------
  bool
  DoublePressModifier::remap(RemapParams& remapParams, const KeyCode& fromKeyCode, const ModifierFlag& toFlag, const KeyCode& fireKeyCode, const Flags& fireFlags)
  {
    if (remapParams.isremapped || remapParams.params.key != fromKeyCode) {
      pressCount_ = 0;
      return false;
    }

    // ----------------------------------------
    bool isKeyDown = remapParams.isKeyDownOrModifierDown();

    const KeyCode& toKeyCode = toFlag.getKeyCode();
    keytokey_.remap(remapParams, fromKeyCode, toKeyCode);

    if (isKeyDown) {
      ++pressCount_;
    } else {
      if (pressCount_ >= 2) {
        Flags flags = (FlagStatus::makeFlags() | fireFlags).stripNONE();
        RemapUtil::fireKey_downup(flags, fireKeyCode, remapParams.params.keyboardType, remapParams.workspacedata);
      }
    }

    return true;
  }

  // ----------------------------------------
  bool
  ModifierHoldingKeyToKey::remap(RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags, const KeyCode& toKeyCode)
  {
    if (remapParams.isremapped || remapParams.params.key != fromKeyCode) {
      IOLog("nottargetkey\n");
      goto nottargetkey;
    }

    if (isAnyEventHappen_) goto nottargetkey;
    if (! FlagStatus::makeFlags().isOn(fromFlags)) goto nottargetkey;

    EventWatcher::unset(isAnyEventHappen_);
    if (! ic_.checkThreshold(config.parameter_modifierholdingkeytokey_wait)) goto nottargetkey;

    return keytokey_.remap(remapParams, fromKeyCode, fromFlags, toKeyCode);

  nottargetkey:
    ic_.begin();
    EventWatcher::set(isAnyEventHappen_);
    return false;
  }

  // ----------------------------------------
  bool
  IgnoreMultipleSameKeyPress::remap(RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags)
  {
    if (remapParams.isremapped || ! FlagStatus::makeFlags().isOn(fromFlags)) {
      lastkeycode_ = KeyCode::VK_NONE.get();
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
      lastkeycode_ = remapParams.params.key.get();
    }
    return false;
  }

  // ------------------------------------------------------------
  bool
  PointingRelativeToScroll::remap(RemapPointingParams_relative& remapParams, const Buttons& buttons, const Flags& fromFlags)
  {
    if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;

    if (buttons == PointingButton::NONE) {
      FlagStatus::temporary_decrease(fromFlags);
      RemapUtil::pointingRelativeToScroll(remapParams);
      return true;
    }

    if (remapParams.params.buttons.isOn(buttons)) {
      // if the source buttons contains left button, we cancel left click for iPhoto, or some applications.
      // iPhoto store the scroll events when left button is pressed, and restore events after left button is released.
      // PointingRelativeToScroll doesn't aim it, we release the left button and do normal scroll event.
      if (buttons.isOn(PointingButton::LEFT)) {
        if (! isButtonHeldDown_) {
          RemapUtil::fireRelativePointer(Params_RelativePointerEventCallback(PointingButton::NONE, 0, 0));
        }
      }

      isButtonHeldDown_ = true;
      FlagStatus::temporary_decrease(fromFlags);
      RemapUtil::pointingRelativeToScroll(remapParams);

      return true;

    } else {
      // ignore button up event.
      if (isButtonHeldDown_) {
        isButtonHeldDown_ = false;
        remapParams.isremapped = true;

        return true;
      }

      return false;
    }
  }

  const Buttons&
  RemapUtil::getRemappedButtons(void)
  {
    return remappedButtions;
  }
}
