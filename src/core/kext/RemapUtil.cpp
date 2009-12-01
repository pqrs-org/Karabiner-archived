#include "RemapUtil.hpp"
#include "keycode.hpp"
#include "Config.hpp"
#include "util/PointingButtonStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  ListFireConsumerKey listFireConsumerKey;
  ListFireRelativePointer listFireRelativePointer;
  FirePointingScroll firePointingScroll;

  bool
  RemapUtil::isInternalKeyboard(unsigned int keyboardType)
  {
    if (keyboardType == KeyboardType::MACBOOK) return true;
    if (keyboardType == KeyboardType::MACBOOK_COREDUO) return true;
    if (keyboardType == KeyboardType::POWERBOOK) return true;
    if (keyboardType == KeyboardType::POWERBOOK_G4) return true;
    if (keyboardType == KeyboardType::POWERBOOK_G4_TI) return true;
    if (keyboardType == KeyboardType::JIS_MACBOOK) return true;
    if (keyboardType == KeyboardType::JIS_MACBOOK_2008) return true;
    return false;
  }

  KeyCode::KeyCode
  RemapUtil::getModifierKeyCode(ModifierFlag::ModifierFlag flag)
  {
    if (flag == ModifierFlag::CAPSLOCK) return KeyCode::CAPSLOCK;
    if (flag == ModifierFlag::SHIFT_L) return KeyCode::SHIFT_L;
    if (flag == ModifierFlag::SHIFT_R) return KeyCode::SHIFT_R;
    if (flag == ModifierFlag::CONTROL_L) return KeyCode::CONTROL_L;
    if (flag == ModifierFlag::CONTROL_R) return KeyCode::CONTROL_R;
    if (flag == ModifierFlag::OPTION_L) return KeyCode::OPTION_L;
    if (flag == ModifierFlag::OPTION_R) return KeyCode::OPTION_R;
    if (flag == ModifierFlag::COMMAND_L) return KeyCode::COMMAND_L;
    if (flag == ModifierFlag::COMMAND_R) return KeyCode::COMMAND_R;
    if (flag == ModifierFlag::FN) return KeyCode::FN;

    //IOLog("KeyRemap4MacBook::getModifierKeyCode invalid args\n");
    return KeyCode::NONE;
  }

  ModifierFlag::ModifierFlag
  RemapUtil::getKeyCodeModifier(unsigned int keycode) {
    if (keycode == KeyCode::CAPSLOCK) return ModifierFlag::CAPSLOCK;
    if (keycode == KeyCode::SHIFT_L) return ModifierFlag::SHIFT_L;
    if (keycode == KeyCode::SHIFT_R) return ModifierFlag::SHIFT_R;
    if (keycode == KeyCode::CONTROL_L) return ModifierFlag::CONTROL_L;
    if (keycode == KeyCode::CONTROL_R) return ModifierFlag::CONTROL_R;
    if (keycode == KeyCode::OPTION_L) return ModifierFlag::OPTION_L;
    if (keycode == KeyCode::OPTION_R) return ModifierFlag::OPTION_R;
    if (keycode == KeyCode::COMMAND_L) return ModifierFlag::COMMAND_L;
    if (keycode == KeyCode::COMMAND_R) return ModifierFlag::COMMAND_R;
    if (keycode == KeyCode::FN) return ModifierFlag::FN;

    //IOLog("KeyRemap4MacBook::getKeyCodeModifier invalid args\n");
    return ModifierFlag::NONE;
  }

  namespace {
    inline bool isFromFlags(unsigned int flags, unsigned int fromFlags) {
      if (RemapUtil::isModifierOn(fromFlags, ModifierFlag::NONE)) {
        return (flags == ModifierFlag::stripNONE(fromFlags));
      } else {
        return ((flags & fromFlags) == fromFlags);
      }
    }
    void remapFlags(unsigned int fromFlags, unsigned int toFlags, unsigned int toKeyCode = KeyCode::NONE, bool isKeyDown = false) {
      // if to Modifier, we change the flag permanently, else change temporary.
      bool isToModifier = (RemapUtil::getKeyCodeModifier(toKeyCode) != ModifierFlag::NONE);

      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        ModifierFlag::ModifierFlag m = ModifierFlag::list[i];
        FlagStatus::Item* p = FlagStatus::getFlagStatus(m);
        if (! p) continue;

        if (RemapUtil::isModifierOn(fromFlags, m)) {
          if (isToModifier) {
            if (isKeyDown) {
              p->decrease();
            } else {
              p->increase();
            }
          } else {
            // always decrease
            p->temporary_decrease();
          }
        }
        if (RemapUtil::isModifierOn(toFlags, m)) {
          if (isToModifier) {
            if (isKeyDown) {
              p->increase();
            } else {
              p->decrease();
            }
          } else {
            // always increase
            p->temporary_increase();
          }
        }
      }
    }
  }

  bool
  RemapUtil::KeyToKey::remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags, KeyCode::KeyCode toKeyCode, unsigned int toFlags)
  {
    if (! RemapUtil::isKey(remapParams, fromKeyCode)) return false;

    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);
    if (isKeyDown) {
      active_ = false;
      if (! isFromFlags(FlagStatus::makeFlags(remapParams), fromFlags)) return false;
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

    ModifierFlag::ModifierFlag fromModifier = getKeyCodeModifier(fromKeyCode);
    ModifierFlag::ModifierFlag toModifier = getKeyCodeModifier(toKeyCode);

    FlagStatus::Item* fromStatus = FlagStatus::getFlagStatus(fromModifier);
    FlagStatus::Item* toStatus = FlagStatus::getFlagStatus(toModifier);

    if (fromStatus == NULL) {
      if (toStatus == NULL) {
        // key2key

      } else {
        // key2modifier
        if (isEvent_Down(remapParams)) {
          toStatus->increase();
        } else if (isEvent_Up(remapParams)) {
          toStatus->decrease();
        }
        remapParams.params.eventType = KeyEvent::MODIFY;
      }

    } else {
      if (toStatus == NULL) {
        // modifier2key
        if (isModifierOn(remapParams, fromModifier)) {
          fromStatus->decrease();
          remapParams.params.eventType = KeyEvent::DOWN;
        } else {
          fromStatus->increase();
          remapParams.params.eventType = KeyEvent::UP;
        }

      } else {
        // modifier2modifier
        if (isModifierOn(remapParams, fromModifier)) {
          toStatus->increase();
          fromStatus->decrease();
        } else {
          toStatus->decrease();
          fromStatus->increase();
        }
      }
    }

    // ----------------------------------------
    remapParams.params.key = toKeyCode;
    remapParams.isremapped = true;
    remapFlags(fromFlags, toFlags, toKeyCode, isKeyDown);

    return true;
  }

  bool
  RemapUtil::KeyToKey::remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                             KeyCode::KeyCode toKeyCode1, unsigned int toFlags1,
                             KeyCode::KeyCode toKeyCode2, unsigned int toFlags2)
  {
    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);

    bool result = remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1);
    if (! result) return false;

    if (isKeyDown) {
      // calc flags
      remapFlags(toFlags1, toFlags2, toKeyCode2, isKeyDown);
      unsigned int flags = FlagStatus::makeFlags(toKeyCode2);
      ListFireExtraKey::addKey(flags, toKeyCode2);
      // restore flags
      remapFlags(toFlags2, toFlags1, toKeyCode1, isKeyDown);
    }

    return true;
  }

  // ----------
  void
  RemapUtil::keyToPointingButton(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, PointingButton::PointingButton toButton)
  {
    if (! RemapUtil::isKey(remapParams, fromKeyCode)) return;

    ModifierFlag::ModifierFlag fromModifier = getKeyCodeModifier(fromKeyCode);
    FlagStatus::Item* fromStatus = FlagStatus::getFlagStatus(fromModifier);

    if (fromStatus) {
      // clear flags
      if (RemapUtil::isKeyDown(remapParams, fromKeyCode)) {
        fromStatus->decrease();
      } else {
        fromStatus->increase();
      }
    }

    if (RemapUtil::isKeyDown(remapParams, fromKeyCode)) {
      listFireRelativePointer.add(toButton);

      PointingButtonStatus::set(toButton, true);

    } else {
      listFireRelativePointer.add(PointingButton::NONE);

      PointingButtonStatus::set(toButton, false);
    }

    RemapUtil::drop(remapParams);
  }

  bool
  RemapUtil::keyToConsumer(const RemapParams& remapParams,
                           KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                           ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags)
  {
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::NONE), fromFlags)) return false;
    if (! RemapUtil::isKey(remapParams, fromKeyCode)) return false;

    remapFlags(fromFlags, toFlags);

    unsigned int eventType = (RemapUtil::isKeyDown(remapParams, fromKeyCode) ? KeyEvent::DOWN : KeyEvent::UP);
    unsigned int flags = FlagStatus::makeFlags(toKeyCode);
    listFireConsumerKey.add(eventType, flags, toKeyCode);

    RemapUtil::drop(remapParams);
    return true;
  }

  bool
  RemapUtil::consumerToKey(const RemapConsumerParams& remapParams,
                           ConsumerKeyCode::ConsumerKeyCode fromKeyCode, unsigned int fromFlags,
                           KeyCode::KeyCode toKeyCode, unsigned int toFlags)
  {
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::NONE), fromFlags)) return false;
    if (! isKey(remapParams, fromKeyCode)) return false;

    bool isKeyDown = (remapParams.params.eventType == KeyEvent::DOWN);
    remapFlags(fromFlags, toFlags, toKeyCode, isKeyDown);

    if (RemapUtil::getKeyCodeModifier(toKeyCode) != ModifierFlag::NONE) {
      remapParams.params.eventType = KeyEvent::MODIFY;
    }
    remapParams.params.key = ConsumerKeyCode::NONE;
    remapParams.ex_remapKeyCode = toKeyCode;

    return true;
  }

  bool
  RemapUtil::consumerToConsumer(const RemapConsumerParams& remapParams,
                                ConsumerKeyCode::ConsumerKeyCode fromKeyCode, unsigned int fromFlags,
                                ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags)
  {
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::NONE), fromFlags)) return false;
    if (! isKey(remapParams, fromKeyCode)) return false;

    remapFlags(fromFlags, toFlags);

    remapParams.params.key = toKeyCode;
    remapParams.params.flavor = toKeyCode;
    return true;
  }

  bool
  RemapUtil::pointingButtonToPointingButton(const RemapPointingParams_relative& remapParams,
                                            PointingButton::PointingButton fromButton, unsigned int fromFlags,
                                            PointingButton::PointingButton toButton)
  {
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::NONE), fromFlags)) return false;
    if (! (remapParams.params.buttons & fromButton)) return false;

    remapFlags(fromFlags, 0);

    remapParams.params.buttons = (remapParams.params.buttons & ~fromButton);
    remapParams.params.buttons |= toButton;
    return true;
  }

  // --------------------
  void
  RemapUtil::fireModifiers(KeyboardEventCallback callback, const Params_KeyboardEventCallBack& params)
  {
    if (callback == NULL) return;

    static unsigned int lastFlags = 0;
    unsigned int toFlags = params.flags;

    if (lastFlags == toFlags) return;
#if 0
    printf("RemapUtil::fireModifiers from:%x to:%x\n", lastFlags, toFlags);
#endif

    // ----------------------------------------------------------------------
    bool modifierStatus[ModifierFlag::listsize];

    // setup modifierStatus
    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      modifierStatus[i] = RemapUtil::isModifierOn(lastFlags, ModifierFlag::list[i]);
    }

    Params_KeyboardEventCallBack callbackparams = params;
    callbackparams.eventType = KeyEvent::MODIFY;

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

      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        ModifierFlag::ModifierFlag m = ModifierFlag::list[i];
        bool from = RemapUtil::isModifierOn(lastFlags, m);
        bool to = RemapUtil::isModifierOn(toFlags, m);

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

        unsigned int flags = 0;
        for (int j = 0; j < ModifierFlag::listsize; ++j) {
          if (modifierStatus[j]) {
            flags |= ModifierFlag::list[j];
          }
        }

        callbackparams.flags = flags;
        callbackparams.key = RemapUtil::getModifierKeyCode(m);
        callbackparams.apply(callback);
      }
    }

    lastFlags = toFlags;
  }

  // ------------------------------------------------------------
  namespace {
    bool
    handle_VK_JIS_TOGGLE_EISUU_KANA(KeyboardEventCallback callback, Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

      // It is necessary to save toKeyCode for KeyUp.
      static KeyCode::KeyCode newkeycode = KeyCode::NONE;

      if (params.eventType == KeyEvent::DOWN) {
        if (workspacedata.inputmode == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE) {
          newkeycode = KeyCode::JIS_EISUU;
        } else {
          newkeycode = KeyCode::JIS_KANA;
        }
      }

      params.key = newkeycode;
      RemapUtil::fireKey(callback, params, workspacedata);
      return true;
    }

    bool
    handle_VK_JIS_EISUU_x2(KeyboardEventCallback callback, Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (params.key != KeyCode::VK_JIS_EISUU_x2) return false;

      if (params.eventType == KeyEvent::DOWN) {
        params.key = KeyCode::JIS_EISUU;
        for (int i = 0; i < 2; ++i) {
          params.eventType = KeyEvent::DOWN;
          RemapUtil::fireKey(callback, params, workspacedata);
          params.eventType = KeyEvent::UP;
          RemapUtil::fireKey(callback, params, workspacedata);
        }
      }
      return true;
    }

    bool
    handle_VK_JIS_KANA_x2(KeyboardEventCallback callback, Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (params.key != KeyCode::VK_JIS_KANA_x2) return false;

      if (params.eventType == KeyEvent::DOWN) {
        params.key = KeyCode::JIS_KANA;
        for (int i = 0; i < 2; ++i) {
          params.eventType = KeyEvent::DOWN;
          RemapUtil::fireKey(callback, params, workspacedata);
          params.eventType = KeyEvent::UP;
          RemapUtil::fireKey(callback, params, workspacedata);
        }
      }
      return true;
    }
  }

  void
  RemapUtil::fireKey(KeyboardEventCallback callback, const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (! callback) return;
    if (params.key == KeyCode::NONE) return;

    // ----------------------------------------
    // handle virtual keys
    Params_KeyboardEventCallBack p = params;
    if (handle_VK_JIS_TOGGLE_EISUU_KANA(callback, p, workspacedata)) return;
    if (handle_VK_JIS_EISUU_x2(callback, p, workspacedata)) return;
    if (handle_VK_JIS_KANA_x2(callback, p, workspacedata)) return;

    // ------------------------------------------------------------
    KeyCode::reverseNormalizeKey(p.key, p.flags, p.keyboardType);

    RemapUtil::fireModifiers(callback, p);

    if (p.eventType == KeyEvent::DOWN || p.eventType == KeyEvent::UP) {
      p.apply(callback);

      if (p.eventType == KeyEvent::DOWN) {
        PressDownKeys::add(p.key, p.keyboardType);
      } else {
        PressDownKeys::remove(p.key, p.keyboardType);
      }
    }
  }

  void
  RemapUtil::fireConsumer(KeyboardSpecialEventCallback callback, const Params_KeyboardSpecialEventCallback& params)
  {
    if (! callback) return;
    if (params.key == ConsumerKeyCode::NONE) return;

    Params_KeyboardSpecialEventCallback p = params;
    p.apply(callback);
  }

  // --------------------
  void
  RemapUtil::pointingRelativeToScroll(const RemapPointingParams_relative& remapParams)
  {
    remapParams.ex_dropEvent = true;

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

    firePointingScroll.set(deltaAxis1, deltaAxis2, 0,
                           fixedDelta1, fixedDelta2, 0,
                           pointDelta1, pointDelta2, 0);
  }

  // ----------------------------------------------------------------------
  void
  FireFunc::firefunc_escape(const RemapParams& params)
  {
    unsigned int flags = FlagStatus::makeFlags(params);
    ListFireExtraKey::addKey(flags, KeyCode::ESCAPE);
  }

  void
  FireFunc::firefunc_return(const RemapParams& params)
  {
    unsigned int flags = FlagStatus::makeFlags(params);
    ListFireExtraKey::addKey(flags, KeyCode::RETURN);
  }

  // ----------------------------------------
  void
  KeyOverlaidModifier::remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, KeyCode::KeyCode fireKeyCode, unsigned int fireFlags, bool isFireRepeat)
  {
    if (! RemapUtil::isKey(remapParams, fromKeyCode)) {
      if (! RemapUtil::isEvent_Up(remapParams)) {
        useAsModifier = true;
      }
      return;
    }

    // ----------------------------------------
    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);

    KeyCode::KeyCode toKeyCode = RemapUtil::getModifierKeyCode(toFlag);
    keytokey_.remap(remapParams, fromKeyCode, toKeyCode);

    // ----------------------------------------
    if (isKeyDown) {
      useAsModifier = false;
      EventWatcher::set(&isClick);
      ic.begin();

      if (isFireRepeat) {
        FlagStatus::temporary_decrease(toFlag);
        unsigned int flags = ModifierFlag::stripNONE(FlagStatus::makeFlags(remapParams) | fireFlags);
        FlagStatus::temporary_increase(toFlag);

        remapParams.ex_repeatKeyCode = fireKeyCode;
        remapParams.ex_repeatFlags = flags;
      }

    } else {
      if (useAsModifier == false && isClick == false) {
        if (remapParams.ex_extraRepeatCounter == 0) {
          if (config.parameter_keyoverlaidmodifier_timeout <= 0 || ic.checkThreshold(config.parameter_keyoverlaidmodifier_timeout) == false) {
            unsigned int flags = ModifierFlag::stripNONE(FlagStatus::makeFlags(remapParams) | fireFlags);
            ListFireExtraKey::addKey(flags, fireKeyCode);
          }
        }
      }
      EventWatcher::unset(&isClick);
    }
  }

  void
  KeyOverlaidModifierCombination::remap(const RemapParams& remapParams, ModifierFlag::ModifierFlag fromFlag1, ModifierFlag::ModifierFlag fromFlag2, FireFunc::FireFunc firefunc)
  {
    KeyCode::KeyCode keyCode1 = RemapUtil::getModifierKeyCode(fromFlag1);
    KeyCode::KeyCode keyCode2 = RemapUtil::getModifierKeyCode(fromFlag2);
    if (keyCode1 == KeyCode::NONE || keyCode2 == KeyCode::NONE) return;

    FlagStatus::Item* fromStatus1 = FlagStatus::getFlagStatus(fromFlag1);
    FlagStatus::Item* fromStatus2 = FlagStatus::getFlagStatus(fromFlag2);
    if (fromStatus1 == NULL || fromStatus2 == NULL) return;

    if (remapParams.params.key != static_cast<unsigned int>(keyCode2)) {
      isCallFireFunc = false;
      return;
    }

    if (fromStatus2->isHeldDown()) {
      isCallFireFunc = true;
      EventWatcher::set(&isClick);

    } else {
      if (fromStatus1->isHeldDown() && isCallFireFunc && isClick == false) {
        fromStatus1->temporary_decrease();
        firefunc(remapParams);
      }
      EventWatcher::unset(&isClick);
    }
  }

  void
  DoublePressModifier::remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, KeyCode::KeyCode fireKeyCode, unsigned int fireFlags)
  {
    if (! RemapUtil::isKey(remapParams, fromKeyCode)) {
      pressCount = 0;
      return;
    }

    // ----------------------------------------
    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);

    KeyCode::KeyCode toKeyCode = RemapUtil::getModifierKeyCode(toFlag);
    keytokey_.remap(remapParams, fromKeyCode, toKeyCode);

    if (isKeyDown) {
      ++pressCount;
    } else {
      if (pressCount >= 2) {
        unsigned int flags = ModifierFlag::stripNONE(FlagStatus::makeFlags(remapParams) | fireFlags);
        ListFireExtraKey::addKey(flags, fireKeyCode);
      }
    }
  }

  // ----------------------------------------
  void
  ModifierHoldingKeyToKey::remap(const RemapParams& remapParams, ModifierFlag::ModifierFlag fromFlag, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode)
  {
    FlagStatus::Item* fromStatus = FlagStatus::getFlagStatus(fromFlag);
    if (fromStatus == NULL) return;

    KeyCode::KeyCode fromFlagKeyCode = fromStatus->getKeyCode();
    if (remapParams.params.key == static_cast<unsigned int>(fromFlagKeyCode)) {
      doremap = false;
      first = fromStatus->isHeldDown();
      ic.begin();
      return;
    }

    if (first) {
      first = false;
      if (ic.checkThreshold(config.parameter_modifierholdingkeytokey_wait)) {
        doremap = true;
      } else {
        doremap = false;
      }
    }

    if (! RemapUtil::isKey(remapParams, fromKeyCode)) return;
    if (! doremap) return;

    fromStatus->temporary_decrease();
    keytokey_.remap(remapParams, fromKeyCode, toKeyCode);
  }

  // ----------------------------------------
  bool
  IgnoreMultipleSameKeyPress::remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags)
  {
    if (RemapUtil::isAlreadyRemapped(remapParams) || FlagStatus::makeFlags(remapParams) != fromFlags) {
      lastkeycode_ = KeyCode::NONE;
      return false;
    }

    if (RemapUtil::isKey(remapParams, fromKeyCode) &&
        static_cast<unsigned int>(fromKeyCode) == lastkeycode_) {
      // disable event.
      RemapUtil::drop(remapParams);
      return true;
    }

    // set lastkeycode_ if KeyUp.
    if (! RemapUtil::isKeyDown(remapParams, fromKeyCode)) {
      lastkeycode_ = remapParams.params.key;
    }
    return false;
  }

  // ----------------------------------------
  void
  FirePointingScroll::fire(ScrollWheelEventCallback callback, OSObject* target, IOHIPointing* pointing, AbsoluteTime ts)
  {
    if (! enable) return;
    enable = false;

    if (callback == NULL) return;

    callback(target,
             deltaAxis1, deltaAxis2, deltaAxis3,
             fixedDelta1, fixedDelta2, fixedDelta3,
             pointDelta1, pointDelta2, pointDelta3,
             0, ts, pointing, 0);

    if (config.debug_pointing) {
#if __x86_64__
      const char* format = "sending scrollWheelEventCallback: deltaAxis(%d, %d, %d), fixedDelta(%ld, %ld, %ld), pointDelta(%d,%d,%d)\n";
#else
      const char* format = "sending scrollWheelEventCallback: deltaAxis(%d, %d, %d), fixedDelta(%d, %d, %d), pointDelta(%d,%d,%d)\n";
#endif
      printf(format,
             deltaAxis1, deltaAxis2, deltaAxis3,
             fixedDelta1, fixedDelta2, fixedDelta3,
             pointDelta1, pointDelta2, pointDelta3);
    }
  }

  // ------------------------------------------------------------
  bool
  PointingRelativeToScroll::remap(const RemapPointingParams_relative& remapParams, unsigned int button, unsigned int fromFlags)
  {
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::NONE), fromFlags)) return false;

    if (button == PointingButton::NONE) {
      RemapUtil::pointingRelativeToScroll(remapParams);
      remapFlags(fromFlags, 0);
      return true;
    }

    if ((remapParams.params.buttons & button) == button) {
      // if the source button contains left button, we cancel left click for iPhoto, or some applications.
      // iPhoto store the scroll events when left button is pressed, and restore events after left button is released.
      // PointingRelativeToScroll doesn't aim it, we release the left button and do normal scroll event.
      if (button & PointingButton::LEFT) {
        if (! isButtonHeldDown) {
          listFireRelativePointer.add(PointingButton::NONE);
        }
      }

      isButtonHeldDown = true;
      RemapUtil::pointingRelativeToScroll(remapParams);

      remapFlags(fromFlags, 0);
      return true;

    } else {
      // ignore button up event.
      if (isButtonHeldDown) {
        isButtonHeldDown = false;
        remapParams.ex_dropEvent = true;

        remapFlags(fromFlags, 0);
        return true;
      }

      return false;
    }
  }
}
