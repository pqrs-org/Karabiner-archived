#include <sys/systm.h>

#include "RemapUtil.hpp"
#include "keycode.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  AllFlagStatus allFlagStatus;
  ListFireExtraKey listFireExtraKey;
  ListFireConsumerKey listFireConsumerKey;
  ListFirePointingClick listFirePointingClick;
  FirePointingScroll firePointingScroll;
  ClickWatcher clickWatcher;
  PointingButtonStatus pointingButtonStatus;
  JISKanaMode jisKanaMode;

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

  void
  RemapUtil::normalizeKeyBeforeRemap(unsigned int *key, unsigned int *flags, unsigned int keyboardType)
  {
    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (*key == KeyCode::ENTER_POWERBOOK) { *key = KeyCode::ENTER; }
    }

    if (RemapUtil::isModifierOn(*flags, ModifierFlag::FN)) {
      if (*key == KeyCode::KEYPAD_0) { *key = KeyCode::M; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_1) { *key = KeyCode::J; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_2) { *key = KeyCode::K; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_3) { *key = KeyCode::L; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_4) { *key = KeyCode::U; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_5) { *key = KeyCode::I; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_6) { *key = KeyCode::O; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_7) { *key = KeyCode::KEY_7; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_8) { *key = KeyCode::KEY_8; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_9) { *key = KeyCode::KEY_9; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_CLEAR) { *key = KeyCode::KEY_6; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_PLUS) { *key = KeyCode::SLASH; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_MINUS) { *key = KeyCode::SEMICOLON; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_MULTIPLY) { *key = KeyCode::P; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_SLASH) { *key = KeyCode::KEY_0; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_EQUAL) { *key = KeyCode::MINUS; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::KEYPAD_DOT) { *key = KeyCode::DOT; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::PAGEUP) { *key = KeyCode::CURSOR_UP; *flags = (stripModifierFN(*flags) | ModifierFlag::CURSOR); }
      if (*key == KeyCode::PAGEDOWN) { *key = KeyCode::CURSOR_DOWN; *flags = (stripModifierFN(*flags) | ModifierFlag::CURSOR); }
      if (*key == KeyCode::HOME) { *key = KeyCode::CURSOR_LEFT; *flags = (stripModifierFN(*flags) | ModifierFlag::CURSOR); }
      if (*key == KeyCode::END) { *key = KeyCode::CURSOR_RIGHT; *flags = (stripModifierFN(*flags) | ModifierFlag::CURSOR); }
      if (*key == KeyCode::ENTER) { *key = KeyCode::RETURN; *flags = stripModifierFN(*flags); }
      if (*key == KeyCode::FORWARD_DELETE) { *key = KeyCode::DELETE; *flags = stripModifierFN(*flags); }
    }
  }

  bool
  RemapUtil::keyToKey(const RemapParams &params, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags, KeyCode::KeyCode toKeyCode, unsigned int toFlags)
  {
    // ----------------------------------------
    if (isModifierOn(fromFlags, ModifierFlag::NONE)) {
      unsigned int flags = stripModifierNone(fromFlags);
      if (allFlagStatus.makeFlags(params) != flags) return false;
    } else {
      unsigned int flags = allFlagStatus.makeFlags(params);
      if ((flags & fromFlags) != fromFlags) return false;
    }

    // ----------------------------------------
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    ModifierFlag::ModifierFlag fromFlag = getKeyCodeModifier(fromKeyCode);
    ModifierFlag::ModifierFlag toFlag = getKeyCodeModifier(toKeyCode);

    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    FlagStatus *toStatus = allFlagStatus.getFlagStatus(toFlag);

    if (fromStatus == NULL) {
      if (toStatus == NULL) {
        // key2key

      } else {
        // key2modifier
        if (*(params.eventType) == KeyEvent::DOWN) {
          toStatus->increase();
        } else if (*(params.eventType) == KeyEvent::UP) {
          toStatus->decrease();
        }
        *(params.eventType) = KeyEvent::MODIFY;
      }

    } else {
      if (toStatus == NULL) {
        // modifier2key
        if (isModifierOn(params, fromFlag)) {
          fromStatus->decrease();
          *(params.eventType) = KeyEvent::DOWN;
        } else {
          fromStatus->increase();
          *(params.eventType) = KeyEvent::UP;
        }

      } else {
        // modifier2modifier
        if (isModifierOn(params, fromFlag)) {
          toStatus->increase();
          fromStatus->decrease();
        } else {
          toStatus->decrease();
          fromStatus->increase();
        }
      }
    }

    *(params.key) = toKeyCode;

    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      ModifierFlag::ModifierFlag m = ModifierFlag::list[i];
      FlagStatus *p = allFlagStatus.getFlagStatus(m);
      if (! p) continue;

      if (isModifierOn(fromFlags, m)) {
        p->temporary_decrease();
      }
      if (isModifierOn(toFlags, m)) {
        p->temporary_increase();
      }
    }

    return true;
  }

  namespace {
    void
    keyToKeyCombination(const RemapParams &params, KeyCode::KeyCode fromKeyCode,
                        KeyCode::KeyCode toKeyCode1, CharCode::CharCode toCharCode1,
                        KeyCode::KeyCode toKeyCode2, CharCode::CharCode toCharCode2)
    {
      if (! RemapUtil::isKeyDown(params, fromKeyCode)) return;

      unsigned int flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.addKey(flags, toKeyCode1, toCharCode1);
      if (toKeyCode2 != KeyCode::NONE) {
        listFireExtraKey.addKey(flags, toKeyCode2, toCharCode2);
      }
      *(params.key) = KeyCode::NONE;

      ExtraRepeatFunc::register_keyCombination(toKeyCode1, toCharCode1, toKeyCode2, toCharCode2);
      *(params.ex_extraRepeatFunc) = ExtraRepeatFunc::extraRepeatFunc_keyCombination;
      *(params.ex_extraRepeatFlags) = flags;
    }
  }

  bool
  RemapUtil::keyToKey_dependingShift(const RemapParams &params, KeyCode::KeyCode fromKeyCode,
                                     KeyCode::KeyCode toKeyCode_noflag1, CharCode::CharCode toCharCode_noflag1,
                                     KeyCode::KeyCode toKeyCode_noflag2, CharCode::CharCode toCharCode_noflag2,
                                     KeyCode::KeyCode toKeyCode_shiftL1, CharCode::CharCode toCharCode_shiftL1,
                                     KeyCode::KeyCode toKeyCode_shiftL2, CharCode::CharCode toCharCode_shiftL2,
                                     KeyCode::KeyCode toKeyCode_shiftR1, CharCode::CharCode toCharCode_shiftR1,
                                     KeyCode::KeyCode toKeyCode_shiftR2, CharCode::CharCode toCharCode_shiftR2)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    if (allFlagStatus.shiftL.isHeldDown()) {
      allFlagStatus.shiftL.temporary_decrease();
      keyToKeyCombination(params, fromKeyCode, toKeyCode_shiftL1, toCharCode_shiftL1, toKeyCode_shiftL2, toCharCode_shiftL2);

    } else if (allFlagStatus.shiftR.isHeldDown()) {
      allFlagStatus.shiftR.temporary_decrease();
      keyToKeyCombination(params, fromKeyCode, toKeyCode_shiftR1, toCharCode_shiftR1, toKeyCode_shiftR2, toCharCode_shiftR2);

    } else {
      keyToKeyCombination(params, fromKeyCode, toKeyCode_noflag1, toCharCode_noflag1, toKeyCode_noflag2, toCharCode_noflag2);
    }

    return true;
  }

  // ----------
  void
  RemapUtil::keyToPointingButton(const RemapParams &params, KeyCode::KeyCode fromKeyCode, PointingButton::PointingButton toButton)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return;

    ModifierFlag::ModifierFlag fromFlag = getKeyCodeModifier(fromKeyCode);
    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);

    if (fromStatus) {
      // clear flags
      if (RemapUtil::isKeyDown(params, fromKeyCode)) {
        fromStatus->decrease();
      } else {
        fromStatus->increase();
      }
    }

    if (RemapUtil::isKeyDown(params, fromKeyCode)) {
      listFirePointingClick.add(toButton);

      bool *status = pointingButtonStatus.getButtonStatus(toButton);
      if (status) *status = true;

    } else {
      listFirePointingClick.add(PointingButton::NONE);

      bool *status = pointingButtonStatus.getButtonStatus(toButton);
      if (status) *status = false;
    }

    *(params.key) = KeyCode::NONE;
  }

  bool
  RemapUtil::keyToConsumer(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ConsumerKeyCode::ConsumerKeyCode toKeyCode)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;
    listFireConsumerKey.add(*(params.eventType), 0, toKeyCode);
    *(params.key) = KeyCode::NONE;
    return true;
  }

  bool
  RemapUtil::consumerToKey(const RemapConsumerParams &params, ConsumerKeyCode::ConsumerKeyCode fromKeyCode, KeyCode::KeyCode toKeyCode)
  {
    if (*(params.key) != fromKeyCode) return false;

    // strip FN modifier
    *(params.flags) = *(params.flags) & (~ModifierFlag::FN);
    *(params.ex_dropKey) = true;
    *(params.ex_remapKey) = true;
    *(params.ex_remapKeyCode) = toKeyCode;
    return true;
  }

  void
  RemapUtil::ejectToKey(const RemapConsumerParams &params, KeyCode::KeyCode toKeyCode)
  {
    if (*(params.key) != ConsumerKeyCode::EJECT) return;

    if (config.option_retain_eject_controlL && allFlagStatus.controlL.isHeldDown()) {
      *(params.flags) = ModifierFlag::CONTROL_L;
      return;
    }
    if (config.option_retain_eject_shiftL && allFlagStatus.shiftL.isHeldDown()) {
      *(params.flags) = 0;
      return;
    }
    if (config.option_retain_eject_shiftR && allFlagStatus.shiftR.isHeldDown()) {
      *(params.flags) = 0;
      return;
    }

    consumerToKey(params, ConsumerKeyCode::EJECT, toKeyCode);
  }

  // --------------------
  namespace {
    void
    fireModifiers(unsigned int fromFlags, unsigned int toFlags,
                  KeyboardEventCallback callback, OSObject *target,
                  unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon)
    {
#if 0
      printf("RemapUtil::fireModifiers from:%x to:%x\n", fromFlags, toFlags);
#endif
      if (callback == NULL) return;
      if (fromFlags == toFlags) return;

      // ----------------------------------------------------------------------
      bool modifierStatus[ModifierFlag::listsize];

      // setup modifierStatus
      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        modifierStatus[i] = RemapUtil::isModifierOn(fromFlags, ModifierFlag::list[i]);
      }

      // fire
      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        ModifierFlag::ModifierFlag m = ModifierFlag::list[i];
        bool from = RemapUtil::isModifierOn(fromFlags, m);
        bool to = RemapUtil::isModifierOn(toFlags, m);

        if (from == to) continue;

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

        KeyCode::KeyCode keyCode = RemapUtil::getModifierKeyCode(m);
        callback(target, KeyEvent::MODIFY, flags, keyCode, 0, 0, 0, 0, keyboardType, false, ts, sender, refcon);

        if (config.debug) {
          printf("sending hid event type KeyEvent::MODIFY flags 0x%x key %d kbdType %d\n", flags, keyCode, keyboardType);
        }
      }
    }

    // reverse convertion of RemapUtil::normalizeKeyBeforeRemap
    void
    reverseNormalizeKey(unsigned int *key, unsigned int *flags, unsigned int keyboardType)
    {
      if (RemapUtil::isModifierOn(*flags, ModifierFlag::FN)) {
        if (*key == KeyCode::M) { *key = KeyCode::KEYPAD_0; }
        if (*key == KeyCode::J) { *key = KeyCode::KEYPAD_1; }
        if (*key == KeyCode::K) { *key = KeyCode::KEYPAD_2; }
        if (*key == KeyCode::L) { *key = KeyCode::KEYPAD_3; }
        if (*key == KeyCode::U) { *key = KeyCode::KEYPAD_4; }
        if (*key == KeyCode::I) { *key = KeyCode::KEYPAD_5; }
        if (*key == KeyCode::O) { *key = KeyCode::KEYPAD_6; }
        if (*key == KeyCode::KEY_7) { *key = KeyCode::KEYPAD_7; }
        if (*key == KeyCode::KEY_8) { *key = KeyCode::KEYPAD_8; }
        if (*key == KeyCode::KEY_9) { *key = KeyCode::KEYPAD_9; }
        if (*key == KeyCode::KEY_6) { *key = KeyCode::KEYPAD_CLEAR; }
        if (*key == KeyCode::SLASH) { *key = KeyCode::KEYPAD_PLUS; }
        if (*key == KeyCode::SEMICOLON) { *key = KeyCode::KEYPAD_MINUS; }
        if (*key == KeyCode::P) { *key = KeyCode::KEYPAD_MULTIPLY; }
        if (*key == KeyCode::KEY_0) { *key = KeyCode::KEYPAD_SLASH; }
        if (*key == KeyCode::MINUS) { *key = KeyCode::KEYPAD_EQUAL; }
        if (*key == KeyCode::DOT) { *key = KeyCode::KEYPAD_DOT; }
        if (*key == KeyCode::CURSOR_UP) { *key = KeyCode::PAGEUP; }
        if (*key == KeyCode::CURSOR_DOWN) { *key = KeyCode::PAGEDOWN; }
        if (*key == KeyCode::CURSOR_LEFT) { *key = KeyCode::HOME; }
        if (*key == KeyCode::CURSOR_RIGHT) { *key = KeyCode::END; }
        if (*key == KeyCode::RETURN) { *key = KeyCode::ENTER; }
        if (*key == KeyCode::DELETE) { *key = KeyCode::FORWARD_DELETE; }
      }
      if (keyboardType == KeyboardType::POWERBOOK ||
          keyboardType == KeyboardType::POWERBOOK_G4 ||
          keyboardType == KeyboardType::POWERBOOK_G4_TI) {
        if (*key == KeyCode::ENTER) { *key = KeyCode::ENTER_POWERBOOK; }
      }
    }
  }

  void
  RemapUtil::fireKey(KeyboardEventCallback callback,
                     OSObject *target, unsigned int eventType, unsigned int flags, unsigned int key, unsigned int charCode,
                     unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet, unsigned int keyboardType,
                     bool repeat, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (! callback) return;
    if (key == KeyCode::NONE) return;

    static unsigned int lastFlags = 0;
    fireModifiers(lastFlags, flags, callback, target, keyboardType, ts, sender, refcon);
    lastFlags = flags;

    if (eventType == KeyEvent::DOWN || eventType == KeyEvent::UP) {
      reverseNormalizeKey(&key, &flags, keyboardType);

      if (config.debug) {
        printf("sending hid event type %d flags 0x%x key %d ", eventType, flags, key);
        printf("charCode %d charSet %d ", charCode, charSet);
        printf("origCharCode %d origCharSet %d kbdType %d\n",
               origCharCode, origCharSet, keyboardType);
      }
      callback(target, eventType, flags, key, charCode,
               charSet, origCharCode, origCharSet, keyboardType, repeat, ts, sender, refcon);

      if (key == KeyCode::JIS_EISUU || key == KeyCode::JIS_KANA) {
        jisKanaMode.setMode(eventType, key, flags);
      }
    }
  }

  // --------------------
  void
  RemapUtil::jis_toggle_eisuu_kana(const RemapParams &params, KeyCode::KeyCode fromKeyCode)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return;

    if (RemapUtil::isKeyDown(params, fromKeyCode)) jisKanaMode.toggle();

    KeyCode::KeyCode toKeyCode = jisKanaMode.iskana() ? KeyCode::JIS_KANA : KeyCode::JIS_EISUU;
    RemapUtil::keyToKey(params, fromKeyCode, toKeyCode);
  }

  // --------------------
  void
  IntervalChecker::begin(void)
  {
    clock_get_system_microtime(&secs, &microsecs);
  }

  bool
  IntervalChecker::checkThreshold(uint32_t millisec)
  {
    uint32_t s;
    uint32_t m;
    clock_get_system_microtime(&s, &m);

    uint32_t interval = static_cast<int>(s - secs) * 1000 + static_cast<int>(m - microsecs) / 1000;
    //printf("checkThreshold interval %d\n", interval);
    return interval >= millisec;
  }

  // --------------------
  void
  RemapUtil::pointingRelativeToScroll(const RemapPointingParams_relative &params)
  {
    *(params.ex_dropEvent) = true;

    // ----------------------------------------
    int delta1 = - *(params.dy);
    int delta2 = - *(params.dx);

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
  FlagStatus::initialize(const RemapParams &params)
  {
    temporary_count = 0;

    if (*(params.eventType) != KeyEvent::MODIFY) return;
    if (params.ex_origKey != key) return;

    if (RemapUtil::isModifierOn(params, flag)) {
      increase();
    } else {
      decrease();
    }
  }

  void
  AllFlagStatus::initialize(const RemapParams &params)
  {
    if (*(params.eventType) == KeyEvent::DOWN) ++numHeldDownKeys;
    if (*(params.eventType) == KeyEvent::UP) --numHeldDownKeys;

#if 0
    if (config.debug) {
      printf("KeyRemap4MacBook numHeldDownKeys = %d\n", numHeldDownKeys);
    }
#endif

    capslock.initialize(params);
    shiftL.initialize(params);
    shiftR.initialize(params);
    controlL.initialize(params);
    controlR.initialize(params);
    optionL.initialize(params);
    optionR.initialize(params);
    commandL.initialize(params);
    commandR.initialize(params);
    fn.initialize(params);
  }

  void
  AllFlagStatus::reset(void)
  {
#if 0
    if (config.debug) {
      printf("KeyRemap4MacBook::AllFlagStatus::reset\n");
    }
#endif

    capslock.reset();
    shiftL.reset();
    shiftR.reset();
    controlL.reset();
    controlR.reset();
    optionL.reset();
    optionR.reset();
    commandL.reset();
    commandR.reset();
    fn.reset();
    numHeldDownKeys = 0;
  }

  unsigned int
  AllFlagStatus::makeFlags(unsigned int keyCode)
  {
    unsigned int flags = 0;
    flags |= capslock.makeFlag();
    flags |= shiftL.makeFlag();
    flags |= shiftR.makeFlag();
    flags |= controlL.makeFlag();
    flags |= controlR.makeFlag();
    flags |= optionL.makeFlag();
    flags |= optionR.makeFlag();
    flags |= commandL.makeFlag();
    flags |= commandR.makeFlag();
    flags |= fn.makeFlag();

    if (keyCode == KeyCode::KEYPAD_0 || keyCode == KeyCode::KEYPAD_1 || keyCode == KeyCode::KEYPAD_2 ||
        keyCode == KeyCode::KEYPAD_3 || keyCode == KeyCode::KEYPAD_4 || keyCode == KeyCode::KEYPAD_5 ||
        keyCode == KeyCode::KEYPAD_6 || keyCode == KeyCode::KEYPAD_7 || keyCode == KeyCode::KEYPAD_8 ||
        keyCode == KeyCode::KEYPAD_9 ||
        keyCode == KeyCode::KEYPAD_DOT ||
        keyCode == KeyCode::KEYPAD_MULTIPLY ||
        keyCode == KeyCode::KEYPAD_PLUS ||
        keyCode == KeyCode::KEYPAD_CLEAR ||
        keyCode == KeyCode::KEYPAD_SLASH ||
        keyCode == KeyCode::KEYPAD_MINUS ||
        keyCode == KeyCode::KEYPAD_EQUAL ||
        keyCode == KeyCode::KEYPAD_COMMA) {
      flags |= ModifierFlag::KEYPAD;
    }

    if (keyCode == KeyCode::CURSOR_UP ||
        keyCode == KeyCode::CURSOR_DOWN ||
        keyCode == KeyCode::CURSOR_LEFT ||
        keyCode == KeyCode::CURSOR_RIGHT) {
      flags |= ModifierFlag::CURSOR;
    }

    return flags;
  }

  unsigned int
  AllFlagStatus::makeFlags(const RemapParams &params)
  {
    if (numHeldDownKeys <= 0 && *(params.flags) == 0) {
      reset();
    }

    return makeFlags(*(params.key));
  }

  FlagStatus *
  AllFlagStatus::getFlagStatus(ModifierFlag::ModifierFlag flag) {
    if (flag == ModifierFlag::CAPSLOCK) return &capslock;
    if (flag == ModifierFlag::SHIFT_L) return &shiftL;
    if (flag == ModifierFlag::SHIFT_R) return &shiftR;
    if (flag == ModifierFlag::CONTROL_L) return &controlL;
    if (flag == ModifierFlag::CONTROL_R) return &controlR;
    if (flag == ModifierFlag::OPTION_L) return &optionL;
    if (flag == ModifierFlag::OPTION_R) return &optionR;
    if (flag == ModifierFlag::COMMAND_L) return &commandL;
    if (flag == ModifierFlag::COMMAND_R) return &commandR;
    if (flag == ModifierFlag::FN) return &fn;
    return NULL;
  }

  FlagStatus *
  AllFlagStatus::getFlagStatus(KeyCode::KeyCode keyCode)
  {
    ModifierFlag::ModifierFlag fromFlag = RemapUtil::getKeyCodeModifier(keyCode);
    return allFlagStatus.getFlagStatus(fromFlag);
  }

  // ----------------------------------------------------------------------
  void
  ListFireExtraKey::fire(KeyboardEventCallback callback,
                         OSObject *target,
                         unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet, unsigned int keyboardType,
                         AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    for (int i = 0; i < size; ++i) {
      FireExtraKey &item = list[i];

      RemapUtil::fireKey(callback, target, item.getEventType(), item.getFlags(), item.getKey(), item.getCharCode(),
                         charSet, origCharCode, origCharSet,
                         keyboardType, false, ts, sender, refcon);
    }
  }

  void
  ListFireConsumerKey::fire(KeyboardSpecialEventCallback callback, OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    for (int i = 0; i < size; ++i) {
      FireConsumerKey &item = list[i];
      unsigned int flavor = item.getKey();
      unsigned int guid = -1;
      callback(target, item.getEventType(), item.getFlags(), item.getKey(), flavor, guid, false, ts, sender, refcon);
    }
  }

  // ----------------------------------------------------------------------
  void
  FireFunc::firefunc_nop(const RemapParams &params)
  {
  }

  void
  FireFunc::firefunc_backslash(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::BACKSLASH, CharCode::BACKSLASH);
  }

  void
  FireFunc::firefunc_commandO(const RemapParams &params)
  {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.add(KeyEvent::DOWN, ModifierFlag::COMMAND_L, KeyCode::O, CharCode::O);
    listFireExtraKey.add(KeyEvent::UP,   ModifierFlag::COMMAND_L, KeyCode::O, CharCode::O);
  }

  void
  FireFunc::firefunc_commandSpace(const RemapParams &params)
  {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.add(KeyEvent::DOWN, ModifierFlag::COMMAND_L, KeyCode::SPACE, CharCode::SPACE);
    listFireExtraKey.add(KeyEvent::UP,   ModifierFlag::COMMAND_L, KeyCode::SPACE, CharCode::SPACE);
  }

  void
  FireFunc::firefunc_enter(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::ENTER, CharCode::ENTER);
  }

  void
  FireFunc::firefunc_escape(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::ESCAPE, CharCode::ESCAPE);
  }

  void
  FireFunc::firefunc_exposeAll(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::EXPOSE_ALL, CharCode::EXPOSE_ALL);
  }

  void
  FireFunc::firefunc_return(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::RETURN, CharCode::RETURN);
  }

  void
  FireFunc::firefunc_space(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::SPACE, CharCode::SPACE);
  }

  void
  FireFunc::firefunc_tab(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::TAB, CharCode::TAB);
  }

  void
  FireFunc::firefunc_emacsmode_controlK(const RemapParams &params, bool first)
  {
    static int counter = 0;
    if (first) counter = 0;

    if (counter % 2 == 0) {
      // Command+Shift+Right
      listFireExtraKey.add(KeyEvent::DOWN, ModifierFlag::COMMAND_L | ModifierFlag::SHIFT_L, KeyCode::CURSOR_RIGHT, CharCode::CURSOR_RIGHT);
      listFireExtraKey.add(KeyEvent::UP,   ModifierFlag::COMMAND_L | ModifierFlag::SHIFT_L, KeyCode::CURSOR_RIGHT, CharCode::CURSOR_RIGHT);

      // Command+X
      listFireExtraKey.add(KeyEvent::DOWN, ModifierFlag::COMMAND_L, KeyCode::X, CharCode::X);
      listFireExtraKey.add(KeyEvent::UP,   ModifierFlag::COMMAND_L, KeyCode::X, CharCode::X);

    } else {
      // Forward Delete
      listFireExtraKey.add(KeyEvent::DOWN, 0, KeyCode::FORWARD_DELETE, CharCode::X);
      listFireExtraKey.add(KeyEvent::UP,   0, KeyCode::FORWARD_DELETE, CharCode::X);
    }

    ++counter;
  }

  void
  FireFunc::firefunc_emacsmode_controlK_2nd(const RemapParams &params)
  {
    firefunc_emacsmode_controlK(params, false);
  }

  void
  FireFunc::firefunc_emacsmode_ex_controlU(const RemapParams &params)
  {
    // Command+Shift+Left
    listFireExtraKey.add(KeyEvent::DOWN, ModifierFlag::COMMAND_L | ModifierFlag::SHIFT_L, KeyCode::CURSOR_LEFT, CharCode::CURSOR_LEFT);
    listFireExtraKey.add(KeyEvent::UP,   ModifierFlag::COMMAND_L | ModifierFlag::SHIFT_L, KeyCode::CURSOR_LEFT, CharCode::CURSOR_LEFT);

    // Command+X
    listFireExtraKey.add(KeyEvent::DOWN, ModifierFlag::COMMAND_L, KeyCode::X, CharCode::X);
    listFireExtraKey.add(KeyEvent::UP,   ModifierFlag::COMMAND_L, KeyCode::X, CharCode::X);
  }

  void
  FireFunc::firefunc_jis_kana(const RemapParams &params)
  {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.addKey(0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
  }

  void
  FireFunc::firefunc_jis_kana_x2(const RemapParams &params)
  {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.addKey(0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
    listFireExtraKey.addKey(0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
  }

  void
  FireFunc::firefunc_jis_eisuu(const RemapParams &params)
  {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.addKey(0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
  }

  void
  FireFunc::firefunc_jis_eisuu_x2(const RemapParams &params)
  {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.addKey(0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
    listFireExtraKey.addKey(0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
  }

  void
  FireFunc::firefunc_jis_toggle_eisuu_kana(const RemapParams &params)
  {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    jisKanaMode.toggle();

    if (jisKanaMode.iskana()) {
      listFireExtraKey.addKey(0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
    } else {
      listFireExtraKey.addKey(0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
    }
  }

  void
  FireFunc::firefunc_jis_eisuu_escape(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
    listFireExtraKey.addKey(flags, KeyCode::ESCAPE, CharCode::ESCAPE);
  }

  // --------------------
  void
  ExtraRepeatFunc::extraRepeatFunc_enter(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    unsigned int charSet = 0;
    unsigned origCharCode = 0;
    unsigned origCharSet = 0;

    RemapUtil::fireKey(callback, target, KeyEvent::DOWN, flags, KeyCode::ENTER, CharCode::ENTER, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
    RemapUtil::fireKey(callback, target, KeyEvent::UP,   flags, KeyCode::ENTER, CharCode::ENTER, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
  }

  void
  ExtraRepeatFunc::extraRepeatFunc_space(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    unsigned int charSet = 0;
    unsigned origCharCode = 0;
    unsigned origCharSet = 0;
    RemapUtil::fireKey(callback, target, KeyEvent::DOWN, flags, KeyCode::SPACE, CharCode::SPACE, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
    RemapUtil::fireKey(callback, target, KeyEvent::UP,   flags, KeyCode::SPACE, CharCode::SPACE, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
  }

  void
  ExtraRepeatFunc::call_firefunc(FireFunc::FireFunc firefunc, KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    unsigned int charSet = 0;
    unsigned origCharCode = 0;
    unsigned origCharSet = 0;

    listFireExtraKey.reset();
    RemapParams params;
    firefunc(params);
    listFireExtraKey.fire(callback, target, charSet, origCharCode, origCharSet, keyboardType, ts, sender, refcon);
  }

  void
  ExtraRepeatFunc::extraRepeatFunc_emacsmode_controlK(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    call_firefunc(FireFunc::firefunc_emacsmode_controlK_2nd, callback, target, flags, keyboardType, ts, sender, refcon);
  }

  namespace ExtraRepeatFunc {
    struct KeyCombination {
      KeyCode::KeyCode keyCode1;
      KeyCode::KeyCode keyCode2;
      CharCode::CharCode charCode1;
      CharCode::CharCode charCode2;
    } keyCombination;
  }

  void
  ExtraRepeatFunc::register_keyCombination(KeyCode::KeyCode keyCode1, CharCode::CharCode charCode1, KeyCode::KeyCode keyCode2, CharCode::CharCode charCode2)
  {
    keyCombination.keyCode1 = keyCode1;
    keyCombination.keyCode2 = keyCode2;
    keyCombination.charCode1 = charCode1;
    keyCombination.charCode2 = charCode2;
  }

  void
  ExtraRepeatFunc::extraRepeatFunc_keyCombination(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    unsigned int charSet = 0;
    unsigned origCharCode = 0;
    unsigned origCharSet = 0;

    if (keyCombination.keyCode1 != KeyCode::NONE) {
      RemapUtil::fireKey(callback, target, KeyEvent::DOWN, flags, keyCombination.keyCode1, keyCombination.charCode1, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
      RemapUtil::fireKey(callback, target, KeyEvent::UP,   flags, keyCombination.keyCode1, keyCombination.charCode1, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
    }
    if (keyCombination.keyCode2 != KeyCode::NONE) {
      RemapUtil::fireKey(callback, target, KeyEvent::DOWN, flags, keyCombination.keyCode2, keyCombination.charCode2, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
      RemapUtil::fireKey(callback, target, KeyEvent::UP,   flags, keyCombination.keyCode2, keyCombination.charCode2, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
    }
  }

  // ----------------------------------------
  namespace {
    bool
    remapToModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag)
    {
      bool isKeyDown = false;
      KeyCode::KeyCode toKeyCode = RemapUtil::getModifierKeyCode(toFlag);

      if (*(params.eventType) == KeyEvent::MODIFY) {
        // remap Modifier to Modifier

        ModifierFlag::ModifierFlag fromFlag = RemapUtil::getKeyCodeModifier(fromKeyCode);
        FlagStatus *status = allFlagStatus.getFlagStatus(fromFlag);
        if (status == NULL) return false;

        if (status->isHeldDown()) {
          isKeyDown = true;
        }
        RemapUtil::keyToKey(params, fromKeyCode, toKeyCode);

      } else {
        // remap Key to Modifier

        if (*(params.eventType) == KeyEvent::DOWN) {
          isKeyDown = true;
        }
        RemapUtil::keyToKey(params, fromKeyCode, toKeyCode);
      }

      return isKeyDown;
    }
  }

  void
  KeyOverlaidModifier::remap(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, FireFunc::FireFunc firefunc, ExtraRepeatFunc::ExtraRepeatFunc extraRepeatFunc)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) {
      if (*(params.eventType) == KeyEvent::DOWN || *(params.eventType) == KeyEvent::MODIFY) {
        useAsModifier = true;
      }
      return;
    }

    // ----------------------------------------
    bool isKeyDown = remapToModifier(params, fromKeyCode, toFlag);

    if (isKeyDown) {
      useAsModifier = false;
      clickWatcher.set(&isClick);
      ic.begin();

      if (extraRepeatFunc) {
        FlagStatus *status = allFlagStatus.getFlagStatus(toFlag);
        if (status) {
          status->temporary_decrease();

          *(params.ex_extraRepeatFunc) = extraRepeatFunc;
          *(params.ex_extraRepeatFlags) = allFlagStatus.makeFlags(params);

          status->temporary_increase();
        }
      }

    } else {
      if (useAsModifier == false && isClick == false) {
        if (extraRepeatFunc == NULL || params.ex_extraRepeatCounter == 0) {
          if (config.parameter_keyoverlaidmodifier_timeout <= 0 || ic.checkThreshold(config.parameter_keyoverlaidmodifier_timeout) == false) {
            firefunc(params);
          }
        }
      }
      clickWatcher.unset(&isClick);
    }
  }

  void
  KeyOverlaidModifierCombination::remap(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag1, ModifierFlag::ModifierFlag fromFlag2, FireFunc::FireFunc firefunc)
  {
    KeyCode::KeyCode keyCode1 = RemapUtil::getModifierKeyCode(fromFlag1);
    KeyCode::KeyCode keyCode2 = RemapUtil::getModifierKeyCode(fromFlag2);
    if (keyCode1 == KeyCode::NONE || keyCode2 == KeyCode::NONE) return;

    FlagStatus *fromStatus1 = allFlagStatus.getFlagStatus(fromFlag1);
    FlagStatus *fromStatus2 = allFlagStatus.getFlagStatus(fromFlag2);
    if (fromStatus1 == NULL || fromStatus2 == NULL) return;

    if (*(params.key) != keyCode2) {
      isCallFireFunc = false;
      return;
    }

    if (fromStatus2->isHeldDown()) {
      isCallFireFunc = true;
      clickWatcher.set(&isClick);

    } else {
      if (fromStatus1->isHeldDown() && isCallFireFunc && isClick == false) {
        fromStatus1->temporary_decrease();
        firefunc(params);
      }
      clickWatcher.unset(&isClick);
    }
  }

  void
  DoublePressModifier::remap(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, FireFunc::FireFunc firefunc)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) {
      pressCount = 0;
      return;
    }

    // ----------------------------------------
    bool isKeyDown = remapToModifier(params, fromKeyCode, toFlag);

    if (isKeyDown) {
      ++pressCount;
    } else {
      if (pressCount >= 2) {
        firefunc(params);
      }
    }
  }

  // ----------------------------------------
  void
  ModifierHoldingKeyToKey::remap(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode)
  {
    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    if (fromStatus == NULL) return;

    KeyCode::KeyCode fromFlagKeyCode = fromStatus->getKeyCode();
    if (*(params.key) == fromFlagKeyCode) {
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

    if (! RemapUtil::isKey(params, fromKeyCode)) return;
    if (! doremap) return;

    fromStatus->temporary_decrease();
    RemapUtil::keyToKey(params, fromKeyCode, toKeyCode);
  }

  // ----------------------------------------
  void
  ClickWatcher::reset(void)
  {
    for (int i = 0; i < CLICKWATCHER_MAXNUM; ++i) {
      watchlist[i] = NULL;
    }
  }

  void
  ClickWatcher::click(void)
  {
    for (int i = 0; i < CLICKWATCHER_MAXNUM; ++i) {
      if (watchlist[i]) {
        *(watchlist[i]) = true;
      }
    }
  }

  void
  ClickWatcher::set(bool *b)
  {
    if (b == NULL) return;

    *b = false;
    for (int i = 0; i < CLICKWATCHER_MAXNUM; ++i) {
      if (watchlist[i] == NULL) {
        watchlist[i] = b;
      }
    }
  }

  void
  ClickWatcher::unset(bool *b)
  {
    if (b == NULL) return;

    for (int i = 0; i < CLICKWATCHER_MAXNUM; ++i) {
      if (watchlist[i] == b) {
        watchlist[i] = NULL;
      }
    }
  }

  // ----------------------------------------
  bool *
  PointingButtonStatus::getButtonStatus(PointingButton::PointingButton button)
  {
    if (button == PointingButton::LEFT) return &helddown_left;
    if (button == PointingButton::RIGHT) return &helddown_right;
    if (button == PointingButton::MIDDLE) return &helddown_middle;
    return NULL;
  }

  // ----------------------------------------
  void
  ListFirePointingClick::add(PointingButton::PointingButton button)
  {
    if (size >= FIREPOINTINGCLICK_MAXNUM) return;
    list[size].set(button);
    ++size;
  }

  void
  ListFirePointingClick::fire(RelativePointerEventCallback callback, OSObject *target, OSObject *sender, AbsoluteTime ts)
  {
    if (callback == NULL) return;

    for (int i = 0; i < size; ++i) {
      FirePointingClick &item = list[i];

      if (config.debug_pointing) {
        printf("sending relativePointerEventCallBack: buttons: %d, ts: 0x%x\n", item.getButton(), ts);
      }
      callback(target, item.getButton(), 0, 0, ts, sender, 0);
    }
  }

  // --------------------
  void
  FirePointingScroll::fire(ScrollWheelEventCallback callback, OSObject *target, IOHIPointing *pointing, AbsoluteTime ts)
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
      printf("sending scrollWheelEventCallback: deltaAxis(%d, %d, %d), fixedDelta(%d, %d, %d), pointDelta(%d,%d,%d)\n",
             deltaAxis1, deltaAxis2, deltaAxis3,
             fixedDelta1, fixedDelta2, fixedDelta3,
             pointDelta1, pointDelta2, pointDelta3);
    }
  }

  // ------------------------------------------------------------
  void
  ButtonRelativeToScroll::remap(const RemapPointingParams_relative &params, PointingButton::PointingButton button)
  {
    if ((*(params.buttons) & button) == 0) {
      if (isButtonHeldDown) {
        isButtonHeldDown = false;
        *(params.ex_dropEvent) = true;
      }
    } else {
      isButtonHeldDown = true;
      *(params.ex_dropEvent) = true;
      RemapUtil::pointingRelativeToScroll(params);
    }
  }

  // ------------------------------------------------------------
  void
  JISKanaMode::setMode(unsigned int eventType, unsigned int keyCode, unsigned int flags)
  {
    if (eventType != KeyEvent::DOWN) return;

    if (keyCode == KeyCode::JIS_EISUU) setMode(JISKANAMODE_ASCII);

    if (keyCode == KeyCode::JIS_KANA) {
      if (RemapUtil::isModifierOn(flags, ModifierFlag::SHIFT_L) ||
          RemapUtil::isModifierOn(flags, ModifierFlag::SHIFT_R)) {
        setMode(JISKANAMODE_KATAKANA);
      } else {
        setMode(JISKANAMODE_HIRAGANA);
      }
    }
  }

  void
  JISKanaMode::toggle(void)
  {
    if (mode == JISKANAMODE_ASCII) {
      if (allFlagStatus.isHeldDown_shift()) {
        setMode(JISKANAMODE_KATAKANA);
      } else {
        setMode(JISKANAMODE_HIRAGANA);
      }
    } else {
      setMode(JISKANAMODE_ASCII);
    }
  }
}
