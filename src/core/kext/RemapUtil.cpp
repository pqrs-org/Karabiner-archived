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
  RemapUtil::fnToNormal(const RemapParams &params)
  {
    if (! RemapUtil::isModifierOn(params, ModifierFlag::FN)) return;

    keyToKey(params, KeyCode::KEYPAD_0, KeyCode::M);
    keyToKey(params, KeyCode::KEYPAD_1, KeyCode::J);
    keyToKey(params, KeyCode::KEYPAD_2, KeyCode::K);
    keyToKey(params, KeyCode::KEYPAD_3, KeyCode::L);
    keyToKey(params, KeyCode::KEYPAD_4, KeyCode::U);
    keyToKey(params, KeyCode::KEYPAD_5, KeyCode::I);
    keyToKey(params, KeyCode::KEYPAD_6, KeyCode::O);
    keyToKey(params, KeyCode::KEYPAD_7, KeyCode::KEY_7);
    keyToKey(params, KeyCode::KEYPAD_8, KeyCode::KEY_8);
    keyToKey(params, KeyCode::KEYPAD_9, KeyCode::KEY_9);
    keyToKey(params, KeyCode::KEYPAD_CLEAR, KeyCode::KEY_6);
    keyToKey(params, KeyCode::KEYPAD_PLUS, KeyCode::SLASH);
    keyToKey(params, KeyCode::KEYPAD_MINUS, KeyCode::SEMICOLON);
    keyToKey(params, KeyCode::KEYPAD_MULTIPLY, KeyCode::P);
    keyToKey(params, KeyCode::KEYPAD_SLASH, KeyCode::KEY_0);
    keyToKey(params, KeyCode::KEYPAD_EQUAL, KeyCode::MINUS);
    keyToKey(params, KeyCode::KEYPAD_DOT, KeyCode::DOT);
    keyToKey(params, KeyCode::PAGEUP, KeyCode::CURSOR_UP);
    keyToKey(params, KeyCode::PAGEDOWN, KeyCode::CURSOR_DOWN);
    keyToKey(params, KeyCode::HOME, KeyCode::CURSOR_LEFT);
    keyToKey(params, KeyCode::END, KeyCode::CURSOR_RIGHT);
    keyToKey(params, KeyCode::ENTER, KeyCode::RETURN);
    keyToKey(params, KeyCode::FORWARD_DELETE, KeyCode::DELETE);
  }

  void
  RemapUtil::toFN(const RemapParams &params) {
    if (! allFlagStatus.fn.isHeldDown()) return;

    keyToKey(params, KeyCode::M, KeyCode::KEYPAD_0);
    keyToKey(params, KeyCode::J, KeyCode::KEYPAD_1);
    keyToKey(params, KeyCode::K, KeyCode::KEYPAD_2);
    keyToKey(params, KeyCode::L, KeyCode::KEYPAD_3);
    keyToKey(params, KeyCode::U, KeyCode::KEYPAD_4);
    keyToKey(params, KeyCode::I, KeyCode::KEYPAD_5);
    keyToKey(params, KeyCode::O, KeyCode::KEYPAD_6);
    keyToKey(params, KeyCode::KEY_7, KeyCode::KEYPAD_7);
    keyToKey(params, KeyCode::KEY_8, KeyCode::KEYPAD_8);
    keyToKey(params, KeyCode::KEY_9, KeyCode::KEYPAD_9);
    keyToKey(params, KeyCode::KEY_6, KeyCode::KEYPAD_CLEAR);
    keyToKey(params, KeyCode::SLASH, KeyCode::KEYPAD_PLUS);
    keyToKey(params, KeyCode::SEMICOLON, KeyCode::KEYPAD_MINUS);
    keyToKey(params, KeyCode::P, KeyCode::KEYPAD_MULTIPLY);
    keyToKey(params, KeyCode::KEY_0, KeyCode::KEYPAD_SLASH);
    keyToKey(params, KeyCode::MINUS, KeyCode::KEYPAD_EQUAL);
    keyToKey(params, KeyCode::DOT, KeyCode::KEYPAD_DOT);
    keyToKey(params, KeyCode::CURSOR_UP, KeyCode::PAGEUP);
    keyToKey(params, KeyCode::CURSOR_DOWN, KeyCode::PAGEDOWN);
    keyToKey(params, KeyCode::CURSOR_LEFT, KeyCode::HOME);
    keyToKey(params, KeyCode::CURSOR_RIGHT, KeyCode::END);
    keyToKey(params, KeyCode::RETURN, KeyCode::ENTER);
    keyToKey(params, KeyCode::DELETE, KeyCode::FORWARD_DELETE);
  }

  void
  RemapUtil::toDelete(const RemapParams &params)
  {
    if (! allFlagStatus.fn.isHeldDown()) return;

    if (*(params.key) == KeyCode::DELETE) {
      *(params.key) = KeyCode::FORWARD_DELETE;
    }
  }

  KeyCode::KeyCode
  RemapUtil::getEnterKeyCode(unsigned int keyboardType)
  {
    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      return KeyCode::ENTER_POWERBOOK;
    }
    return KeyCode::ENTER;
  }

  CharCode::CharCode
  RemapUtil::getEnterCharCode(KeyCode::KeyCode keyCode)
  {
    if (keyCode == KeyCode::ENTER_POWERBOOK) return CharCode::ENTER_POWERBOOK;
    if (keyCode == KeyCode::ENTER) return CharCode::ENTER;
    return CharCode::NONE;
  }

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

  bool
  RemapUtil::keyToKey(const RemapParams &params, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode)
  {
    // ----------------------------------------
    if (fromKeyCode == KeyCode::FN) fnToNormal(params);
    if (toKeyCode == KeyCode::FN) toFN(params);

    if (fromKeyCode == KeyCode::ENTER) fromKeyCode = getEnterKeyCode(params);
    if (toKeyCode == KeyCode::ENTER) toKeyCode = getEnterKeyCode(params);

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
    if (toKeyCode == KeyCode::DELETE) toDelete(params);

    return true;
  }

  bool
  RemapUtil::keyToKeyWithModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode, ModifierFlag::ModifierFlag toFlag)
  {
    if (fromKeyCode == KeyCode::ENTER) fromKeyCode = getEnterKeyCode(params);
    if (toKeyCode == KeyCode::ENTER) toKeyCode = getEnterKeyCode(params);

    // ----------------------------------------
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    FlagStatus *toStatus = allFlagStatus.getFlagStatus(toFlag);
    if (toStatus == NULL) return false;

    unsigned int charCode = 0;
    if (RemapUtil::isKeyDown(params, fromKeyCode)) {
      toStatus->increase();
      unsigned int flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_BEFORE, KeyEvent::MODIFY, flags, toStatus->getKeyCode(), charCode);

    } else {
      toStatus->decrease();
      unsigned int flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, toStatus->getKeyCode(), charCode);

      toStatus->temporary_increase();
    }

    keyToKey(params, fromKeyCode, toKeyCode);
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
      *(params.ex_dropKey) = true;

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

    *(params.ex_dropKey) = true;

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
  }

  bool
  RemapUtil::keyToConsumer(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ConsumerKeyCode::ConsumerKeyCode toKeyCode)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;
    listFireConsumerKey.add(*(params.eventType), 0, toKeyCode);
    *(params.ex_dropKey) = true;
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
  void
  RemapUtil::fireModifiers(unsigned int fromFlags, unsigned int toFlags,
                           KeyboardEventCallback callback, OSObject *target,
                           unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

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
          flags |= ModifierFlag::list[i];
        }
      }

      KeyCode::KeyCode keyCode = RemapUtil::getModifierKeyCode(m);
      callback(target, KeyEvent::MODIFY, flags, keyCode, 0, 0, 0, 0, keyboardType, false, ts, sender, refcon);
    }
  }

  bool
  RemapUtil::keypad2spaces(const RemapParams &params)
  {
    // 789      123
    // 456  to  456
    // 123      789
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_1, KeyCode::KEYPAD_7);
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_2, KeyCode::KEYPAD_8);
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_3, KeyCode::KEYPAD_9);

    RemapUtil::keyToKey(params, KeyCode::KEYPAD_7, KeyCode::KEYPAD_1);
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_8, KeyCode::KEYPAD_2);
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_9, KeyCode::KEYPAD_3);

    if (*(params.key) == KeyCode::KEYPAD_1 ||
        *(params.key) == KeyCode::KEYPAD_2 ||
        *(params.key) == KeyCode::KEYPAD_3 ||
        *(params.key) == KeyCode::KEYPAD_4 ||
        *(params.key) == KeyCode::KEYPAD_5 ||
        *(params.key) == KeyCode::KEYPAD_6 ||
        *(params.key) == KeyCode::KEYPAD_7 ||
        *(params.key) == KeyCode::KEYPAD_8 ||
        *(params.key) == KeyCode::KEYPAD_9) {
      return true;
    } else {
      return false;
    }
  }

  bool
  RemapUtil::key2spaces(const RemapParams &params, unsigned int flags,
                        KeyCode::KeyCode key1, KeyCode::KeyCode key2, KeyCode::KeyCode key3,
                        KeyCode::KeyCode key4, KeyCode::KeyCode key5, KeyCode::KeyCode key6,
                        KeyCode::KeyCode key7, KeyCode::KeyCode key8, KeyCode::KeyCode key9,
                        KeyCode::KeyCode key10,
                        KeyCode::KeyCode key11,
                        KeyCode::KeyCode key12,
                        KeyCode::KeyCode key13,
                        KeyCode::KeyCode key14,
                        KeyCode::KeyCode key15,
                        KeyCode::KeyCode key16)
  {
    KeyCode::KeyCode keyCode = KeyCode::NONE;
    CharCode::CharCode charCode = CharCode::NONE;

    // --------------------------------------------------
    if (RemapUtil::isKey(params, key1)) { keyCode = KeyCode::KEY_1; charCode = CharCode::KEY_1; }
    if (RemapUtil::isKey(params, key2)) { keyCode = KeyCode::KEY_2; charCode = CharCode::KEY_2; }
    if (RemapUtil::isKey(params, key3)) { keyCode = KeyCode::KEY_3; charCode = CharCode::KEY_3; }
    if (RemapUtil::isKey(params, key4)) { keyCode = KeyCode::KEY_4; charCode = CharCode::KEY_4; }
    if (RemapUtil::isKey(params, key5)) { keyCode = KeyCode::KEY_5; charCode = CharCode::KEY_5; }
    if (RemapUtil::isKey(params, key6)) { keyCode = KeyCode::KEY_6; charCode = CharCode::KEY_6; }
    if (RemapUtil::isKey(params, key7)) { keyCode = KeyCode::KEY_7; charCode = CharCode::KEY_7; }
    if (RemapUtil::isKey(params, key8)) { keyCode = KeyCode::KEY_8; charCode = CharCode::KEY_8; }
    if (RemapUtil::isKey(params, key9)) { keyCode = KeyCode::KEY_9; charCode = CharCode::KEY_9; }
    if (RemapUtil::isKey(params, key10)) { keyCode = KeyCode::KEY_0; charCode = CharCode::KEY_0; }

    if (keyCode != KeyCode::NONE) {
      allFlagStatus.temporary_set(flags);
      RemapUtil::toKey(params, keyCode);
      return true;
    }

    // --------------------------------------------------
    if (RemapUtil::isKey(params, key11)) { keyCode = KeyCode::KEY_1; charCode = CharCode::KEY_1; }
    if (RemapUtil::isKey(params, key12)) { keyCode = KeyCode::KEY_2; charCode = CharCode::KEY_2; }
    if (RemapUtil::isKey(params, key13)) { keyCode = KeyCode::KEY_3; charCode = CharCode::KEY_3; }
    if (RemapUtil::isKey(params, key14)) { keyCode = KeyCode::KEY_4; charCode = CharCode::KEY_4; }
    if (RemapUtil::isKey(params, key15)) { keyCode = KeyCode::KEY_5; charCode = CharCode::KEY_5; }
    if (RemapUtil::isKey(params, key16)) { keyCode = KeyCode::KEY_6; charCode = CharCode::KEY_6; }

    if (keyCode != KeyCode::NONE) {
      allFlagStatus.temporary_set(flags | ModifierFlag::OPTION_L);
      RemapUtil::toKey(params, keyCode);
      return true;
    }

    return false;
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

  void
  AllFlagStatus::temporary_set(unsigned int flags)
  {
    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      FlagStatus *p = getFlagStatus(ModifierFlag::list[i]);
      if (! p) continue;

      p->temporary_reset();
      if (RemapUtil::isModifierOn(flags, ModifierFlag::list[i])) {
        if (p) p->temporary_increase();
      }
    }
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

  void
  AllFlagStatus::temporary_flags_reset()
  {
    if (commandL.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::COMMAND_L, CharCode::COMMAND_L);
    if (commandR.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::COMMAND_R, CharCode::COMMAND_R);

    if (controlL.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::CONTROL_L, CharCode::CONTROL_L);
    if (controlR.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::CONTROL_R, CharCode::CONTROL_R);

    if (optionL.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::OPTION_L, CharCode::OPTION_L);
    if (optionR.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::OPTION_R, CharCode::OPTION_R);

    if (shiftL.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::SHIFT_L, CharCode::SHIFT_L);
    if (shiftR.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::SHIFT_R, CharCode::SHIFT_R);

    if (fn.isHeldDown()) listFireExtraKey.addKey(0, KeyCode::FN, CharCode::FN);
  }

  void
  AllFlagStatus::temporary_flags_restore(KeyCode::KeyCode keyCode)
  {
    unsigned int flags = makeFlags(keyCode);

    if (commandL.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::COMMAND_L, CharCode::COMMAND_L);
    if (commandR.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::COMMAND_R, CharCode::COMMAND_R);

    if (controlL.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::CONTROL_L, CharCode::CONTROL_L);
    if (controlR.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::CONTROL_R, CharCode::CONTROL_R);

    if (optionL.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::OPTION_L, CharCode::OPTION_L);
    if (optionR.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::OPTION_R, CharCode::OPTION_R);

    if (shiftL.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::SHIFT_L, CharCode::SHIFT_L);
    if (shiftR.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::SHIFT_R, CharCode::SHIFT_R);

    if (fn.isHeldDown()) listFireExtraKey.addKey(flags, KeyCode::FN, CharCode::FN);
  }

  // ----------------------------------------------------------------------
  void
  ListFireExtraKey::fire(FireExtraKey::Type type, KeyboardEventCallback callback,
                         OSObject *target,
                         unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet, unsigned int keyboardType,
                         AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    for (int i = 0; i < size; ++i) {
      FireExtraKey &item = list[i];

      if (item.getType() == type) {
        callback(target, item.getEventType(), item.getFlags(), item.getKey(), item.getCharCode(),
                 charSet, origCharCode, origCharSet,
                 keyboardType, false, ts, sender, refcon);

        if (item.getKey() == KeyCode::JIS_EISUU ||
            item.getKey() == KeyCode::JIS_KANA) {
          jisKanaMode.setMode(item.getEventType(), item.getKey(), item.getFlags());
        }
      }
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

    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, ModifierFlag::COMMAND_L, KeyCode::COMMAND_L, CharCode::COMMAND_L);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN,   ModifierFlag::COMMAND_L, KeyCode::O,         CharCode::O);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP,     ModifierFlag::COMMAND_L, KeyCode::O,         CharCode::O);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, 0,                       KeyCode::COMMAND_L, CharCode::COMMAND_L);
  }

  void
  FireFunc::firefunc_commandSpace(const RemapParams &params)
  {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, ModifierFlag::COMMAND_L, KeyCode::COMMAND_L, CharCode::COMMAND_L);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN,   ModifierFlag::COMMAND_L, KeyCode::SPACE,     CharCode::SPACE);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP,     ModifierFlag::COMMAND_L, KeyCode::SPACE,     CharCode::SPACE);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, 0,                       KeyCode::COMMAND_L, CharCode::COMMAND_L);
  }

  void
  FireFunc::firefunc_enter(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    KeyCode::KeyCode keyCode = RemapUtil::getEnterKeyCode(params);
    CharCode::CharCode charCode = RemapUtil::getEnterCharCode(keyCode);
    listFireExtraKey.addKey(flags, keyCode, charCode);
  }

  void
  FireFunc::firefunc_escape(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::ESCAPE, CharCode::ESCAPE);
  }

  void
  FireFunc::firefunc_escape_noflags(const RemapParams &params)
  {
    allFlagStatus.temporary_flags_reset();
    listFireExtraKey.addKey(0, KeyCode::ESCAPE, CharCode::ESCAPE);
    allFlagStatus.temporary_flags_restore(KeyCode::ESCAPE);
  }

  void
  FireFunc::firefunc_exposeAll(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::EXPOSE_ALL, CharCode::EXPOSE_ALL);
  }

  void
  FireFunc::firefunc_return_noflags(const RemapParams &params)
  {
    allFlagStatus.temporary_flags_reset();
    listFireExtraKey.addKey(0, KeyCode::RETURN, CharCode::RETURN);
    allFlagStatus.temporary_flags_restore(KeyCode::RETURN);
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
      unsigned int flags = ModifierFlag::SHIFT_L;
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::SHIFT_L,      CharCode::SHIFT_L);
      flags |= ModifierFlag::COMMAND_L;
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::COMMAND_L,    CharCode::COMMAND_L);

      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN,   flags, KeyCode::CURSOR_RIGHT, CharCode::CURSOR_RIGHT);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP,     flags, KeyCode::CURSOR_RIGHT, CharCode::CURSOR_RIGHT);

      // release Shift
      flags = ModifierFlag::COMMAND_L;
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::SHIFT_L,      CharCode::SHIFT_L);

      // Command+X
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN,   flags, KeyCode::X,            CharCode::X);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP,     flags, KeyCode::X,            CharCode::X);

      // release Command
      flags = 0;
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::COMMAND_L,    CharCode::COMMAND_L);
    } else {
      // Forward Delete
      unsigned int flags = 0;
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, flags, KeyCode::FORWARD_DELETE, CharCode::X);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP,   flags, KeyCode::FORWARD_DELETE, CharCode::X);
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
    unsigned int flags = ModifierFlag::SHIFT_L;
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::SHIFT_L,     CharCode::SHIFT_L);
    flags |= ModifierFlag::COMMAND_L;
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::COMMAND_L,   CharCode::COMMAND_L);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN,   flags, KeyCode::CURSOR_LEFT, CharCode::CURSOR_LEFT);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP,     flags, KeyCode::CURSOR_LEFT, CharCode::CURSOR_LEFT);

    // Command+X
    flags = ModifierFlag::COMMAND_L;
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::SHIFT_L, CharCode::SHIFT_L);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN,   flags, KeyCode::X,       CharCode::X);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP,     flags, KeyCode::X,       CharCode::X);

    // release Command
    flags = 0;
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::COMMAND_L,    CharCode::COMMAND_L);
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
    callback(target, KeyEvent::DOWN, flags, KeyCode::ENTER, CharCode::ENTER, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
    callback(target, KeyEvent::UP,   flags, KeyCode::ENTER, CharCode::ENTER, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
  }

  void
  ExtraRepeatFunc::extraRepeatFunc_space(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    unsigned int charSet = 0;
    unsigned origCharCode = 0;
    unsigned origCharSet = 0;
    callback(target, KeyEvent::DOWN, flags, KeyCode::SPACE, CharCode::SPACE, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
    callback(target, KeyEvent::UP,   flags, KeyCode::SPACE, CharCode::SPACE, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
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
    listFireExtraKey.fire(FireExtraKey::TYPE_BEFORE, callback, target, charSet, origCharCode, origCharSet, keyboardType, ts, sender, refcon);
    listFireExtraKey.fire(FireExtraKey::TYPE_AFTER,  callback, target, charSet, origCharCode, origCharSet, keyboardType, ts, sender, refcon);
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
      callback(target, KeyEvent::DOWN, flags, keyCombination.keyCode1, keyCombination.charCode1, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
      callback(target, KeyEvent::UP,   flags, keyCombination.keyCode1, keyCombination.charCode1, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
    }
    if (keyCombination.keyCode2 != KeyCode::NONE) {
      callback(target, KeyEvent::DOWN, flags, keyCombination.keyCode2, keyCombination.charCode2, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
      callback(target, KeyEvent::UP,   flags, keyCombination.keyCode2, keyCombination.charCode2, charSet, origCharCode, origCharSet, keyboardType, false, ts, sender, refcon);
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
        firefunc(params);
      }
      clickWatcher.unset(&isClick);
    }
  }

  bool
  KeyWithModifierToKey::remap(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag fromFlag, KeyCode::KeyCode toKeyCode)
  {
    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    if (fromStatus == NULL) return false;

    if (fromStatus->isHeldDown()) {
      if (RemapUtil::keyToKey(params, fromKeyCode, toKeyCode)) {
        fromStatus->temporary_decrease();
        return true;
      }
    }
    return false;
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
