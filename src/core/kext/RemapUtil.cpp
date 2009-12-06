#include "RemapUtil.hpp"
#include "KeyCode.hpp"
#include "Config.hpp"
#include "util/KeyboardRepeat.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedConsumer.hpp"
#include "util/ListHookedPointing.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    Buttons remappedButtions;

    void remapFlags(const Flags& fromFlags, const Flags& toFlags, const KeyCode& toKeyCode = KeyCode::VK_NONE, bool isKeyDown = false) {
      if (toKeyCode.isModifier()) {
        if (isKeyDown) {
          FlagStatus::decrease(fromFlags);
          FlagStatus::increase(toFlags);
        } else {
          FlagStatus::increase(fromFlags);
          FlagStatus::decrease(toFlags);
        }
      } else {
        // we always perform the same movement regardless of isKeyDown.
        FlagStatus::temporary_decrease(fromFlags);
        FlagStatus::temporary_increase(toFlags);
      }
    }
  }

  bool
  RemapUtil::KeyToKey::remap(const RemapParams& remapParams,
                             const KeyCode& fromKeyCode, const Flags& fromFlags,
                             const KeyCode& toKeyCode,   const Flags& toFlags,
                             bool isSetKeyRepeat)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    // ------------------------------------------------------------
    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);
    if (isKeyDown) {
      active_ = false;
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
    const ModifierFlag& fromModifierFlag = fromKeyCode.getModifierFlag();
    const ModifierFlag& toModifierFlag = toKeyCode.getModifierFlag();

    if (fromModifierFlag == ModifierFlag::NONE) {
      if (toModifierFlag == ModifierFlag::NONE) {
        // key2key

      } else {
        // key2modifier
        if (isEvent_Down(remapParams)) {
          FlagStatus::increase(toModifierFlag);
        } else if (isEvent_Up(remapParams)) {
          FlagStatus::decrease(toModifierFlag);
        }
        remapParams.params.eventType = EventType::MODIFY;
      }

    } else {
      if (toModifierFlag == ModifierFlag::NONE) {
        // modifier2key
        if (remapParams.params.flags.isOn(fromModifierFlag)) {
          FlagStatus::decrease(fromModifierFlag);
          remapParams.params.eventType = EventType::DOWN;
        } else {
          FlagStatus::increase(fromModifierFlag);
          remapParams.params.eventType = EventType::UP;
        }

      } else {
        // modifier2modifier
        if (remapParams.params.flags.isOn(fromModifierFlag)) {
          FlagStatus::increase(toModifierFlag);
          FlagStatus::decrease(fromModifierFlag);
        } else {
          FlagStatus::decrease(toModifierFlag);
          FlagStatus::increase(fromModifierFlag);
        }
      }
    }

    // ----------------------------------------
    remapParams.params.key = toKeyCode;
    remapParams.isremapped = true;
    remapFlags(fromFlags, toFlags, toKeyCode, isKeyDown);

    remapParams.params.flags = FlagStatus::makeFlags();
    RemapUtil::fireKey(remapParams.params, remapParams.workspacedata);

    // ----------------------------------------
    if (isSetKeyRepeat) {
      KeyboardRepeat::set(remapParams.params);
    }

    return true;
  }

  bool
  RemapUtil::KeyToKey::remap(const RemapParams& remapParams,
                             const KeyCode& fromKeyCode, const Flags& fromFlags,
                             const KeyCode& toKeyCode1,  const Flags& toFlags1,
                             const KeyCode& toKeyCode2,  const Flags& toFlags2)
  {
    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);

    bool result = remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, false);
    if (! result) return false;

    if (isKeyDown) {
      // calc flags
      remapFlags(toFlags1, toFlags2, toKeyCode2, isKeyDown);

      Flags flags = FlagStatus::makeFlags();
      RemapUtil::fireKey_downup(flags, toKeyCode2, remapParams.params, remapParams.workspacedata);

      // restore flags
      remapFlags(toFlags2, toFlags1, toKeyCode1, isKeyDown);
    }

    return true;
  }

  // ----------
  bool
  RemapUtil::keyToPointingButton(const RemapParams& remapParams, const KeyCode& fromKeyCode, const PointingButton& toButton)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    // ------------------------------------------------------------
    const ModifierFlag& fromModifierFlag = fromKeyCode.getModifierFlag();

    if (fromModifierFlag != ModifierFlag::NONE) {
      // clear flags
      if (RemapUtil::isKeyDown(remapParams, fromKeyCode)) {
        FlagStatus::decrease(fromModifierFlag);
      } else {
        FlagStatus::increase(fromModifierFlag);
      }
    }

    // ------------------------------------------------------------
    if (RemapUtil::isKeyDown(remapParams, fromKeyCode)) {
      RemapUtil::fireRelativePointer(toButton);
      remappedButtions.add(toButton);

    } else {
      RemapUtil::fireRelativePointer(PointingButton::NONE);
      remappedButtions.remove(toButton);
    }

    RemapUtil::drop(remapParams);
    return true;
  }

  bool
  RemapUtil::keyToConsumer(const RemapParams& remapParams,
                           const KeyCode& fromKeyCode, const Flags& fromFlags,
                           const ConsumerKeyCode& toKeyCode, const Flags& toFlags)
  {
    if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
    if (! RemapUtil::isKey(remapParams, fromKeyCode)) return false;

    remapFlags(fromFlags, toFlags);

    HookedConsumer* hc = ListHookedConsumer::instance().get();
    if (hc) {
      EventType eventType = (RemapUtil::isKeyDown(remapParams, fromKeyCode) ? EventType::DOWN : EventType::UP);
      Flags flags = FlagStatus::makeFlags();
      unsigned int flavor = toKeyCode.get();
      UInt64 guid = static_cast<UInt64>(-1);

      Params_KeyboardSpecialEventCallback p = {
        hc->getOrig_keyboardSpecialEventTarget(),
        eventType, flags, toKeyCode, flavor, guid, false, remapParams.params.ts, hc->get(), NULL,
      };
      RemapUtil::fireConsumer(p);
    }

    RemapUtil::drop(remapParams);
    return true;
  }

  bool
  RemapUtil::consumerToKey(const RemapConsumerParams& remapParams,
                           const ConsumerKeyCode& fromKeyCode, const Flags& fromFlags,
                           const KeyCode& toKeyCode, const Flags& toFlags)
  {
    if (remapParams.isremapped) return false;
    if (remapParams.params.key != fromKeyCode) return false;

    // XXX: use KeyToKey instead self implement.
    if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;

    bool isKeyDown = (remapParams.params.eventType == EventType::DOWN);
    remapFlags(fromFlags, toFlags, toKeyCode, isKeyDown);

    EventType eventType = remapParams.params.eventType;
    if (toKeyCode.isModifier()) {
      eventType = EventType::MODIFY;
    }
    RemapUtil::drop(remapParams);

    Flags flags = FlagStatus::makeFlags();
    RemapUtil::fireKey(eventType, flags, toKeyCode,
                       Params_KeyboardEventCallBack::getcurrent_keyboardType(),
                       remapParams.params.ts,
                       remapParams.workspacedata);

    return true;
  }

  bool
  RemapUtil::consumerToConsumer(const RemapConsumerParams& remapParams,
                                const ConsumerKeyCode& fromKeyCode, const Flags& fromFlags,
                                const ConsumerKeyCode& toKeyCode,   const Flags& toFlags)
  {
    if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
    if (! isKey(remapParams, fromKeyCode)) return false;

    remapFlags(fromFlags, toFlags);

    remapParams.params.key = toKeyCode;
    remapParams.params.flavor = toKeyCode.get();
    return true;
  }

  bool
  RemapUtil::pointingButtonToPointingButton(const RemapPointingParams_relative& remapParams,
                                            const PointingButton& fromButton, const Flags& fromFlags,
                                            const PointingButton& toButton)
  {
    if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
    if (! remapParams.params.buttons.isOn(fromButton)) return false;

    remapFlags(fromFlags, 0);

    remapParams.params.buttons.remove(fromButton);
    remapParams.params.buttons.add(toButton);
    return true;
  }

  // --------------------
  Flags FireModifiers::lastFlags_;

  void
  FireModifiers::fire(const Params_KeyboardEventCallBack& params)
  {
    const Flags& toFlags = params.flags;

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

    Params_KeyboardEventCallBack callbackparams = params;
    callbackparams.eventType = EventType::MODIFY;

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

        callbackparams.flags = flags;
        callbackparams.key = m.getKeyCode();
        callbackparams.apply();
      }
    }

    lastFlags_ = toFlags.get();
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
          RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_EISUU, params, workspacedata);
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
          RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_KANA, params, workspacedata);
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

    FireModifiers::fire(p);

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
  RemapUtil::fireKey(const EventType& eventType, const Flags& flags, const KeyCode& key, const KeyboardType& keyboardType, const AbsoluteTime& ts,
                     const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    HookedKeyboard* hk = ListHookedKeyboard::instance().get();
    if (! hk) return;

    EventType et = eventType;
    if (key.isModifier()) {
      et = EventType::MODIFY;
    }

    Params_KeyboardEventCallBack params = {
      hk->getOrig_keyboardEventTarget(), eventType, flags, key,
      0, 0, 0, 0,
      keyboardType, false, ts, hk->get(), NULL,
    };
    RemapUtil::fireKey(params, workspacedata);
  }

  void
  RemapUtil::fireConsumer(const Params_KeyboardSpecialEventCallback& params)
  {
    params.apply();
  }

  void
  RemapUtil::fireRelativePointer(const Buttons& buttons)
  {
    HookedPointing* hp = ListHookedPointing::instance().get();
    if (! hp) return;

    OSObject* target = hp->getOrig_relativePointerEventTarget();
    OSObject* sender = hp->get();
    AbsoluteTime& ts = Params_RelativePointerEventCallback::getcurrent_ts();

    Params_RelativePointerEventCallback params = {
      target, buttons, 0, 0, ts, sender, NULL,
    };
    params.apply();
  }

  void
  RemapUtil::fireScrollWheel(short int deltaAxis1, short int deltaAxis2, short int deltaAxis3,
                             IOFixed fixedDelta1, IOFixed fixedDelta2, IOFixed fixedDelta3,
                             SInt32 pointDelta1, SInt32 pointDelta2, SInt32 pointDelta3)
  {
    HookedPointing* hp = ListHookedPointing::instance().get();
    if (! hp) return;

    OSObject* target = hp->getOrig_scrollWheelEventTarget();
    OSObject* sender = hp->get();
    AbsoluteTime& ts = Params_ScrollWheelEventCallback::getcurrent_ts();

    Params_ScrollWheelEventCallback params = {
      target,
      deltaAxis1, deltaAxis2, deltaAxis3,
      fixedDelta1, fixedDelta2, fixedDelta3,
      pointDelta1, pointDelta2, pointDelta3,
      0, ts, sender, NULL,
    };
    params.apply();
  }

  // --------------------
  void
  RemapUtil::pointingRelativeToScroll(const RemapPointingParams_relative& remapParams)
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
  KeyOverlaidModifier::remap(const RemapParams& remapParams,
                             const KeyCode& fromKeyCode, const Flags& fromFlags,
                             const KeyCode& toKeyCode,   const Flags& toFlags,
                             const KeyCode& fireKeyCode, const Flags& fireFlags,
                             bool isFireRepeat)
  {
    // ----------------------------------------
    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);
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
      remapFlags(toFlags | toKeyCodeFlag, fireFlags);

      savedflags_ = FlagStatus::makeFlags().get();

      // restore flags
      remapFlags(fireFlags, toFlags | toKeyCodeFlag);

      if (isFireRepeat) {
        KeyboardRepeat::set(EventType::DOWN, savedflags_, fireKeyCode, remapParams.params.keyboardType,
                            config.get_keyoverlaidmodifier_initial_wait());
      }

    } else {
      if (savedIsAnyEventHappen == false) {
        if (config.parameter_keyoverlaidmodifier_timeout <= 0 || ic_.checkThreshold(config.parameter_keyoverlaidmodifier_timeout) == false) {
          RemapUtil::fireKey_downup(savedflags_, fireKeyCode, remapParams.params, remapParams.workspacedata);
        }
      }
      EventWatcher::unset(isAnyEventHappen_);
    }

    return true;
  }

  // ----------------------------------------------------------------------
  bool
  DoublePressModifier::remap(const RemapParams& remapParams, const KeyCode& fromKeyCode, const ModifierFlag& toFlag, const KeyCode& fireKeyCode, const Flags& fireFlags)
  {
    if (! RemapUtil::isKey(remapParams, fromKeyCode)) {
      pressCount = 0;
      return false;
    }

    // ----------------------------------------
    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);

    const KeyCode& toKeyCode = toFlag.getKeyCode();
    keytokey_.remap(remapParams, fromKeyCode, toKeyCode);

    if (isKeyDown) {
      ++pressCount;
    } else {
      if (pressCount >= 2) {
        Flags flags = (FlagStatus::makeFlags() | fireFlags).stripNONE();
        RemapUtil::fireKey_downup(flags, fireKeyCode, remapParams.params, remapParams.workspacedata);
      }
    }

    return true;
  }

  // ----------------------------------------
  bool
  ModifierHoldingKeyToKey::remap(const RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags, const KeyCode& toKeyCode)
  {
    if (! FlagStatus::makeFlags().isOn(fromFlags)) {
      isAnyEventHappen_ = true;
      return false;
    }

    if (remapParams.isremapped || remapParams.params.key != fromKeyCode) {
      ic_.begin();
      EventWatcher::set(isAnyEventHappen_);
      return false;
    }

    if (isAnyEventHappen_ == false) {
      EventWatcher::unset(isAnyEventHappen_);
      if (ic_.checkThreshold(config.parameter_modifierholdingkeytokey_wait)) {
        keytokey_.remap(remapParams, fromKeyCode, fromFlags, toKeyCode);
      }
    }

    return true;
  }

  // ----------------------------------------
  bool
  IgnoreMultipleSameKeyPress::remap(const RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags)
  {
    if (RemapUtil::isAlreadyRemapped(remapParams) || FlagStatus::makeFlags() != fromFlags) {
      lastkeycode_ = KeyCode::VK_NONE.get();
      return false;
    }

    if (RemapUtil::isKey(remapParams, fromKeyCode) &&
        fromKeyCode == lastkeycode_) {
      // disable event.
      RemapUtil::drop(remapParams);
      return true;
    }

    // set lastkeycode_ if KeyUp.
    if (! RemapUtil::isKeyDown(remapParams, fromKeyCode)) {
      lastkeycode_ = remapParams.params.key.get();
    }
    return false;
  }

  // ------------------------------------------------------------
  bool
  PointingRelativeToScroll::remap(const RemapPointingParams_relative& remapParams, const Buttons& buttons, const Flags& fromFlags)
  {
    if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;

    if (buttons == PointingButton::NONE) {
      RemapUtil::pointingRelativeToScroll(remapParams);
      remapFlags(fromFlags, 0);
      return true;
    }

    if (remapParams.params.buttons.isOn(buttons)) {
      // if the source buttons contains left button, we cancel left click for iPhoto, or some applications.
      // iPhoto store the scroll events when left button is pressed, and restore events after left button is released.
      // PointingRelativeToScroll doesn't aim it, we release the left button and do normal scroll event.
      if (buttons.isOn(PointingButton::LEFT)) {
        if (! isButtonHeldDown_) {
          RemapUtil::fireRelativePointer(PointingButton::NONE);
        }
      }

      isButtonHeldDown_ = true;
      RemapUtil::pointingRelativeToScroll(remapParams);

      remapFlags(fromFlags, 0);
      return true;

    } else {
      // ignore button up event.
      if (isButtonHeldDown_) {
        isButtonHeldDown_ = false;
        remapParams.isremapped = true;

        remapFlags(fromFlags, 0);
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
