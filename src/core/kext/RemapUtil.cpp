#include "RemapUtil.hpp"
#include "keycode.hpp"
#include "Config.hpp"
#include "util/KeyboardRepeat.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedConsumer.hpp"
#include "util/PointingButtonStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
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
    return KeyCode::VK_NONE;
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
    void remapFlags(unsigned int fromFlags, unsigned int toFlags, unsigned int toKeyCode = KeyCode::VK_NONE, bool isKeyDown = false) {
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
  RemapUtil::KeyToKey::remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags, KeyCode::KeyCode toKeyCode, unsigned int toFlags, bool isSetKeyRepeat)
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

    remapParams.params.flags = FlagStatus::makeFlags(remapParams);
    RemapUtil::fireKey(remapParams.params, remapParams.workspacedata);

    // ----------------------------------------
    if (isSetKeyRepeat) {
      KeyboardRepeat::set(remapParams.params);
    }

    return true;
  }

  bool
  RemapUtil::KeyToKey::remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                             KeyCode::KeyCode toKeyCode1, unsigned int toFlags1,
                             KeyCode::KeyCode toKeyCode2, unsigned int toFlags2)
  {
    bool isKeyDown = RemapUtil::isKeyDown(remapParams, fromKeyCode);

    bool result = remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, false);
    if (! result) return false;

    if (isKeyDown) {
      // calc flags
      remapFlags(toFlags1, toFlags2, toKeyCode2, isKeyDown);

      unsigned int flags = FlagStatus::makeFlags(toKeyCode2);
      RemapUtil::fireKey_downup(flags, toKeyCode2, remapParams.params, remapParams.workspacedata);

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
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::VK_NONE), fromFlags)) return false;
    if (! RemapUtil::isKey(remapParams, fromKeyCode)) return false;

    remapFlags(fromFlags, toFlags);

    HookedConsumer* hc = ListHookedConsumer::instance().get();
    if (hc) {
      unsigned int eventType = (RemapUtil::isKeyDown(remapParams, fromKeyCode) ? KeyEvent::DOWN : KeyEvent::UP);
      unsigned int flags = FlagStatus::makeFlags(toKeyCode);
      unsigned int flavor = toKeyCode;
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
                           ConsumerKeyCode::ConsumerKeyCode fromKeyCode, unsigned int fromFlags,
                           KeyCode::KeyCode toKeyCode, unsigned int toFlags)
  {
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::VK_NONE), fromFlags)) return false;
    if (! isKey(remapParams, fromKeyCode)) return false;

    bool isKeyDown = (remapParams.params.eventType == KeyEvent::DOWN);
    remapFlags(fromFlags, toFlags, toKeyCode, isKeyDown);

    if (RemapUtil::getKeyCodeModifier(toKeyCode) != ModifierFlag::NONE) {
      remapParams.params.eventType = KeyEvent::MODIFY;
    }
    RemapUtil::drop(remapParams);

    unsigned int flags = FlagStatus::makeFlags(toKeyCode);
    RemapUtil::fireKey(remapParams.params.eventType, flags, toKeyCode,
                       Params_KeyboardEventCallBack::getcurrent_keyboardType(),
                       remapParams.params.ts,
                       remapParams.workspacedata);

    return true;
  }

  bool
  RemapUtil::consumerToConsumer(const RemapConsumerParams& remapParams,
                                ConsumerKeyCode::ConsumerKeyCode fromKeyCode, unsigned int fromFlags,
                                ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags)
  {
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::VK_NONE), fromFlags)) return false;
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
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::VK_NONE), fromFlags)) return false;
    if (! (remapParams.params.buttons & fromButton)) return false;

    remapFlags(fromFlags, 0);

    remapParams.params.buttons = (remapParams.params.buttons & ~fromButton);
    remapParams.params.buttons |= toButton;
    return true;
  }

  // --------------------
  void
  RemapUtil::fireModifiers(const Params_KeyboardEventCallBack& params)
  {
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
        callbackparams.apply();
      }
    }

    lastFlags = toFlags;
  }

  // ------------------------------------------------------------
  namespace {
    bool
    handle_VK_JIS_TOGGLE_EISUU_KANA(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

      // It is necessary to save toKeyCode for KeyUp.
      static KeyCode::KeyCode newkeycode = KeyCode::VK_NONE;

      if (params.eventType == KeyEvent::DOWN) {
        if (workspacedata.inputmode == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE) {
          newkeycode = KeyCode::JIS_EISUU;
        } else {
          newkeycode = KeyCode::JIS_KANA;
        }
      }

      params.key = newkeycode;
      RemapUtil::fireKey(params, workspacedata);
      return true;
    }

    bool
    handle_VK_JIS_EISUU_x2(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (params.key != KeyCode::VK_JIS_EISUU_x2) return false;

      if (params.eventType == KeyEvent::DOWN) {
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

      if (params.eventType == KeyEvent::DOWN) {
        for (int i = 0; i < 2; ++i) {
          RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_KANA, params, workspacedata);
        }
      }
      return true;
    }
  }

  void
  RemapUtil::fireKey(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    // ----------------------------------------
    // handle virtual keys
    Params_KeyboardEventCallBack p = params;
    if (handle_VK_JIS_TOGGLE_EISUU_KANA(p, workspacedata)) return;
    if (handle_VK_JIS_EISUU_x2(p, workspacedata)) return;
    if (handle_VK_JIS_KANA_x2(p, workspacedata)) return;

    // ------------------------------------------------------------
    KeyCode::reverseNormalizeKey(p.key, p.flags, p.keyboardType);

    RemapUtil::fireModifiers(p);

    if (p.eventType == KeyEvent::DOWN || p.eventType == KeyEvent::UP) {
      p.apply();

      if (p.eventType == KeyEvent::DOWN) {
        PressDownKeys::add(p.key, p.keyboardType);
      } else {
        PressDownKeys::remove(p.key, p.keyboardType);
      }
    }
  }

  void
  RemapUtil::fireKey(unsigned int eventType, unsigned int flags, unsigned int key, unsigned int keyboardType, const AbsoluteTime& ts,
                     const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    HookedKeyboard* hk = ListHookedKeyboard::instance().get();
    if (! hk) return;

    if (RemapUtil::getKeyCodeModifier(key) != ModifierFlag::NONE) {
      eventType = KeyEvent::MODIFY;
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

    firePointingScroll.set(deltaAxis1, deltaAxis2, 0,
                           fixedDelta1, fixedDelta2, 0,
                           pointDelta1, pointDelta2, 0);
  }

  // ----------------------------------------------------------------------
  bool
  KeyOverlaidModifier::remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                             KeyCode::KeyCode toKeyCode, unsigned int toFlags,
                             KeyCode::KeyCode fireKeyCode, unsigned int fireFlags, bool isFireRepeat)
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
      unsigned int toKeyCodeFlag = RemapUtil::getKeyCodeModifier(toKeyCode);
      // XXX: add FlagStatus::temporary_decrease(unsigned int flags);
      remapFlags(toKeyCodeFlag | toFlags, fireFlags);

      savedflags_ = FlagStatus::makeFlags(fireKeyCode);

      // restore flags
      // XXX: add FlagStatus::temporary_increase(unsigned int flags);
      remapFlags(fireFlags, toKeyCodeFlag | toFlags);

      if (isFireRepeat) {
        KeyboardRepeat::set(KeyEvent::DOWN, savedflags_, fireKeyCode, static_cast<KeyboardType::KeyboardType>(remapParams.params.keyboardType),
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
        RemapUtil::fireKey_downup(flags, fireKeyCode, remapParams.params, remapParams.workspacedata);
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
      lastkeycode_ = KeyCode::VK_NONE;
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
    if (! isFromFlags(FlagStatus::makeFlags(KeyCode::VK_NONE), fromFlags)) return false;

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
        remapParams.isremapped = true;

        remapFlags(fromFlags, 0);
        return true;
      }

      return false;
    }
  }
}
