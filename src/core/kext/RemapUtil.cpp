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

    IOLog("KeyRemap4MacBook::getModifierKeyCode invalid args\n");
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

    IOLog("KeyRemap4MacBook::getKeyCodeModifier invalid args\n");
    return ModifierFlag::NONE;
  }

  void
  RemapUtil::fnToNormal(const RemapParams &params)
  {
    allFlagStatus.keypad = false;
    if (params.ex_origKey == KeyCode::KEYPAD_0) {
      *(params.key) = KeyCode::M;
    } else if (params.ex_origKey == KeyCode::KEYPAD_1) {
      *(params.key) = KeyCode::J;
    } else if (params.ex_origKey == KeyCode::KEYPAD_2) {
      *(params.key) = KeyCode::K;
    } else if (params.ex_origKey == KeyCode::KEYPAD_3) {
      *(params.key) = KeyCode::L;
    } else if (params.ex_origKey == KeyCode::KEYPAD_4) {
      *(params.key) = KeyCode::U;
    } else if (params.ex_origKey == KeyCode::KEYPAD_5) {
      *(params.key) = KeyCode::I;
    } else if (params.ex_origKey == KeyCode::KEYPAD_6) {
      *(params.key) = KeyCode::O;
    } else if (params.ex_origKey == KeyCode::KEYPAD_7) {
      *(params.key) = KeyCode::KEY_7;
    } else if (params.ex_origKey == KeyCode::KEYPAD_8) {
      *(params.key) = KeyCode::KEY_8;
    } else if (params.ex_origKey == KeyCode::KEYPAD_9) {
      *(params.key) = KeyCode::KEY_9;
    } else if (params.ex_origKey == KeyCode::KEYPAD_CLEAR) {
      *(params.key) = KeyCode::KEY_6;
    } else if (params.ex_origKey == KeyCode::KEYPAD_PLUS) {
      *(params.key) = KeyCode::SLASH;
    } else if (params.ex_origKey == KeyCode::KEYPAD_MINUS) {
      *(params.key) = KeyCode::SEMICOLON;
    } else if (params.ex_origKey == KeyCode::KEYPAD_MULTIPLY) {
      *(params.key) = KeyCode::P;
    } else if (params.ex_origKey == KeyCode::KEYPAD_SLASH) {
      *(params.key) = KeyCode::KEY_0;
    } else if (params.ex_origKey == KeyCode::KEYPAD_EQUAL) {
      *(params.key) = KeyCode::MINUS;
    } else if (params.ex_origKey == KeyCode::KEYPAD_DOT) {
      *(params.key) = KeyCode::DOT;
    } else if (params.ex_origKey == KeyCode::PAGEUP) {
      *(params.key) = KeyCode::CURSOR_UP;
      allFlagStatus.cursor = true;
    } else if (params.ex_origKey == KeyCode::PAGEDOWN) {
      *(params.key) = KeyCode::CURSOR_DOWN;
      allFlagStatus.cursor = true;
    } else if (params.ex_origKey == KeyCode::HOME) {
      *(params.key) = KeyCode::CURSOR_LEFT;
      allFlagStatus.cursor = true;
    } else if (params.ex_origKey == KeyCode::END) {
      *(params.key) = KeyCode::CURSOR_RIGHT;
      allFlagStatus.cursor = true;
    } else if (params.ex_origKey == getEnterKeyCode(params)) {
      *(params.key) = KeyCode::RETURN;
    } else if (params.ex_origKey == KeyCode::FORWARD_DELETE) {
      *(params.key) = KeyCode::DELETE;
    } else {
      return;
    }
  }

  void
  RemapUtil::toFN(const RemapParams &params) {
    if (! allFlagStatus.fn.isHeldDown()) return;

    // change cursor & return key behavior
    if (params.ex_origKey == KeyCode::M) {
      *(params.key) = KeyCode::KEYPAD_0;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::J) {
      *(params.key) = KeyCode::KEYPAD_1;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::K) {
      *(params.key) = KeyCode::KEYPAD_2;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::L) {
      *(params.key) = KeyCode::KEYPAD_3;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::U) {
      *(params.key) = KeyCode::KEYPAD_4;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::I) {
      *(params.key) = KeyCode::KEYPAD_5;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::O) {
      *(params.key) = KeyCode::KEYPAD_6;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::KEY_7) {
      *(params.key) = KeyCode::KEYPAD_7;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::KEY_8) {
      *(params.key) = KeyCode::KEYPAD_8;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::KEY_9) {
      *(params.key) = KeyCode::KEYPAD_9;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::KEY_6) {
      *(params.key) = KeyCode::KEYPAD_CLEAR;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::SLASH) {
      *(params.key) = KeyCode::KEYPAD_PLUS;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::SEMICOLON) {
      *(params.key) = KeyCode::KEYPAD_MINUS;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::P) {
      *(params.key) = KeyCode::KEYPAD_MULTIPLY;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::KEY_0) {
      *(params.key) = KeyCode::KEYPAD_SLASH;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::MINUS) {
      *(params.key) = KeyCode::KEYPAD_EQUAL;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::DOT) {
      *(params.key) = KeyCode::KEYPAD_DOT;
      allFlagStatus.keypad = true;
    } else if (params.ex_origKey == KeyCode::CURSOR_UP) {
      *(params.key) = KeyCode::PAGEUP;
    } else if (params.ex_origKey == KeyCode::CURSOR_DOWN) {
      *(params.key) = KeyCode::PAGEDOWN;
    } else if (params.ex_origKey == KeyCode::CURSOR_LEFT) {
      *(params.key) = KeyCode::HOME;
    } else if (params.ex_origKey == KeyCode::CURSOR_RIGHT) {
      *(params.key) = KeyCode::END;
    } else if (params.ex_origKey == KeyCode::RETURN) {
      *(params.key) = getEnterKeyCode(params);
    } else if (params.ex_origKey == KeyCode::DELETE) {
      *(params.key) = KeyCode::FORWARD_DELETE;
    } else {
      return;
    }
    // strip CURSOR flag
    allFlagStatus.cursor = false;
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
  RemapUtil::modifierToModifier(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, ModifierFlag::ModifierFlag toFlag)
  {
    KeyCode::KeyCode fromKeyCode = getModifierKeyCode(fromFlag);
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    if (fromStatus == NULL) return false;
    FlagStatus *toStatus = allFlagStatus.getFlagStatus(toFlag);
    if (toStatus == NULL) return false;

    if (isModifierOn(params, fromFlag)) {
      toStatus->increase();
      fromStatus->decrease();
    } else {
      toStatus->decrease();
      fromStatus->increase();
    }

    KeyCode::KeyCode toKeyCode = getModifierKeyCode(toFlag);
    *(params.key) = toKeyCode;

    return true;
  }

  bool
  RemapUtil::modifierToKey(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, KeyCode::KeyCode toKeyCode)
  {
    KeyCode::KeyCode fromKeyCode = getModifierKeyCode(fromFlag);
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    if (fromStatus == NULL) return false;

    if (isModifierOn(params, fromFlag)) {
      fromStatus->decrease();
      *(params.eventType) = KeyEvent::DOWN;
    } else {
      fromStatus->increase();
      *(params.eventType) = KeyEvent::UP;
    }

    *(params.key) = toKeyCode;

    if (toKeyCode == KeyCode::DELETE) {
      toDelete(params);
    }

    return true;
  }

  bool
  RemapUtil::keyToModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    FlagStatus *toStatus = allFlagStatus.getFlagStatus(toFlag);
    if (toStatus == NULL) return false;

    if (*(params.eventType) == KeyEvent::DOWN) {
      toStatus->increase();
    } else if (*(params.eventType) == KeyEvent::UP) {
      toStatus->decrease();
    }

    KeyCode::KeyCode toKeyCode = getModifierKeyCode(toFlag);
    *(params.key) = toKeyCode;
    *(params.eventType) = KeyEvent::MODIFY;

    return true;
  }

  bool
  RemapUtil::keyToModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag1, ModifierFlag::ModifierFlag toFlag2)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    FlagStatus *toStatus1 = allFlagStatus.getFlagStatus(toFlag1);
    FlagStatus *toStatus2 = allFlagStatus.getFlagStatus(toFlag2);
    if (toStatus1 == NULL || toStatus2 == NULL) return false;

    if (*(params.eventType) == KeyEvent::DOWN) {
      toStatus1->increase();
      toStatus2->increase();
    } else if (*(params.eventType) == KeyEvent::UP) {
      toStatus1->decrease();
      toStatus2->decrease();
    }

    KeyCode::KeyCode toKeyCode = getModifierKeyCode(toFlag1);
    *(params.key) = toKeyCode;
    *(params.eventType) = KeyEvent::MODIFY;

    return true;
  }

  bool
  RemapUtil::keyToKey(const RemapParams &params, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;
    *(params.key) = toKeyCode;

    if (toKeyCode == KeyCode::DELETE) toDelete(params);

    return true;
  }

  bool
  RemapUtil::keyToKeyWithModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode, ModifierFlag::ModifierFlag toFlag)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    FlagStatus *toStatus = allFlagStatus.getFlagStatus(toFlag);
    if (toStatus == NULL) return false;

    unsigned int charCode = 0;
    if (*(params.eventType) == KeyEvent::DOWN) {
      toStatus->increase();
      unsigned int flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_BEFORE, KeyEvent::MODIFY, flags, toStatus->getKeyCode(), charCode);
    } else if (*(params.eventType) == KeyEvent::UP) {
      toStatus->decrease();
      unsigned int flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, toStatus->getKeyCode(), charCode);

      toStatus->temporary_increase();
    }

    *(params.key) = toKeyCode;
    return true;
  }

  bool
  RemapUtil::keyToKey(const RemapParams &params, KeyCode::KeyCode fromKeyCode,
                      KeyCode::KeyCode toKeyCode1, CharCode::CharCode toCharCode1,
                      KeyCode::KeyCode toKeyCode2, CharCode::CharCode toCharCode2)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return false;

    if (toKeyCode2 == KeyCode::NONE) {
      RemapUtil::keyToKey(params, fromKeyCode, toKeyCode1);

    } else {
      if (RemapUtil::isKeyDown(params, fromKeyCode)) {
        unsigned int flags = allFlagStatus.makeFlags(params);
        listFireExtraKey.addKey(flags, toKeyCode1, toCharCode1);
        listFireExtraKey.addKey(flags, toKeyCode2, toCharCode2);
        *(params.ex_dropKey) = true;
      }
    }

    return true;
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
      RemapUtil::keyToKey(params, fromKeyCode, toKeyCode_shiftL1, toCharCode_shiftL1, toKeyCode_shiftL2, toCharCode_shiftL2);

    } else if (allFlagStatus.shiftR.isHeldDown()) {
      allFlagStatus.shiftR.temporary_decrease();
      RemapUtil::keyToKey(params, fromKeyCode, toKeyCode_shiftR1, toCharCode_shiftR1, toKeyCode_shiftR2, toCharCode_shiftR2);

    } else {
      RemapUtil::keyToKey(params, fromKeyCode, toKeyCode_noflag1, toCharCode_noflag1, toKeyCode_noflag2, toCharCode_noflag2);
    }

    return true;
  }

  // ----------
  void
  RemapUtil::modifierToPointingButton(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, PointingButton::PointingButton toButton)
  {
    KeyCode::KeyCode fromKeyCode = getModifierKeyCode(fromFlag);
    if (! RemapUtil::isKey(params, fromKeyCode)) return;

    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    if (fromStatus == NULL) return;

    *(params.ex_dropKey) = true;

    if (isModifierOn(params, fromFlag)) {
      fromStatus->decrease();
      listFirePointingClick.add(toButton);

      bool *status = pointingButtonStatus.getButtonStatus(toButton);
      if (status) *status = true;

    } else {
      fromStatus->increase();
      listFirePointingClick.add(PointingButton::NONE);

      bool *status = pointingButtonStatus.getButtonStatus(toButton);
      if (status) *status = false;
    }
  }

  void
  RemapUtil::keyToPointingButton(const RemapParams &params, KeyCode::KeyCode fromKeyCode, PointingButton::PointingButton toButton)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return;

    *(params.ex_dropKey) = true;

    if (*(params.eventType) == KeyEvent::DOWN) {
      listFirePointingClick.add(toButton);

      bool *status = pointingButtonStatus.getButtonStatus(toButton);
      if (status) *status = true;

    } else if (*(params.eventType) == KeyEvent::UP) {
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
    if (*(params.key) == ConsumerKeyCode::EJECT) {
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

      *(params.ex_dropKey) = true;
      *(params.ex_remapKey) = true;
      *(params.ex_remapKeyCode) = toKeyCode;
    }
  }

  // --------------------
  void
  RemapUtil::fireKeyWithAllModifiers(const RemapParams &params, unsigned int eventType, unsigned int keyCode, unsigned int charCode)
  {
    unsigned int flags = 0;
    if (eventType == KeyEvent::DOWN) {
      // ----------------------------------------
      // COMMAND_L
      allFlagStatus.commandL.temporary_increase();
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::COMMAND_L, CharCode::COMMAND_L);
      // CONTROL_L
      allFlagStatus.controlL.temporary_increase();
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::CONTROL_L, CharCode::CONTROL_L);
      // OPTION_L
      allFlagStatus.optionL.temporary_increase();
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::OPTION_L, CharCode::OPTION_L);
      // SHIFT_L
      allFlagStatus.shiftL.temporary_increase();
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::SHIFT_L, CharCode::SHIFT_L);

      // ----------------------------------------
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, flags, keyCode, charCode);

    } else if (eventType == KeyEvent::UP) {
      allFlagStatus.commandL.temporary_increase();
      allFlagStatus.controlL.temporary_increase();
      allFlagStatus.optionL.temporary_increase();
      allFlagStatus.shiftL.temporary_increase();

      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, flags, keyCode, charCode);

      // ----------------------------------------
      // COMMAND_L
      allFlagStatus.commandL.temporary_decrease();
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::COMMAND_L, CharCode::COMMAND_L);
      // CONTROL_L
      allFlagStatus.controlL.temporary_decrease();
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::CONTROL_L, CharCode::CONTROL_L);
      // OPTION_L
      allFlagStatus.optionL.temporary_decrease();
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::OPTION_L, CharCode::OPTION_L);
      // SHIFT_L
      allFlagStatus.shiftL.temporary_decrease();
      flags = allFlagStatus.makeFlags(params);
      listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, flags, KeyCode::SHIFT_L, CharCode::SHIFT_L);
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

  // --------------------
  void
  RemapUtil::jis_toggle_eisuu_kana(const RemapParams &params, KeyCode::KeyCode fromKeyCode)
  {
    if (! RemapUtil::isKey(params, fromKeyCode)) return;

    if (RemapUtil::isKeyDown(params, fromKeyCode)) jisKanaMode.toggle();

    KeyCode::KeyCode toKeyCode = jisKanaMode.iskana() ? KeyCode::JIS_KANA : KeyCode::JIS_EISUU;
    RemapUtil::keyToKey(params, fromKeyCode, toKeyCode);
  }

  void
  RemapUtil::jis_toggle_eisuu_kana(const RemapParams &params, ModifierFlag::ModifierFlag fromModifier)
  {
    KeyCode::KeyCode fromKeyCode = getModifierKeyCode(fromModifier);

    if (! RemapUtil::isKey(params, fromKeyCode)) return;

    if (RemapUtil::isKeyDown(params, fromKeyCode)) jisKanaMode.toggle();

    KeyCode::KeyCode toKeyCode = jisKanaMode.iskana() ? KeyCode::JIS_KANA : KeyCode::JIS_EISUU;
    RemapUtil::modifierToKey(params, fromModifier, toKeyCode);
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

    uint32_t interval = (s - secs) * 1000 + (m - microsecs) / 1000;
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

    // *** Note *** ModifierFlag::CURSOR == ModifierFlag::KEYPAD
    if (RemapUtil::isModifierOn(params, ModifierFlag::CURSOR)) {
      if (params.ex_origKey == KeyCode::CURSOR_UP ||
          params.ex_origKey == KeyCode::CURSOR_DOWN ||
          params.ex_origKey == KeyCode::CURSOR_LEFT ||
          params.ex_origKey == KeyCode::CURSOR_RIGHT) {
        cursor = true;
      } else {
        keypad = true;
      }
    } else {
      cursor = keypad = false;
    }
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
    cursor = false;
    keypad = false;
    numHeldDownKeys = 0;
  }

  unsigned int
  AllFlagStatus::makeFlags(const RemapParams &params)
  {
    if (numHeldDownKeys <= 0 && *(params.flags) == 0) {
      reset();
    }

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

    if (cursor) {
      flags |= ModifierFlag::CURSOR;
    }
    if (keypad) {
      flags |= ModifierFlag::KEYPAD;
    }
    return flags;
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

        if (item.getKey() == KeyCode::JIS_EISUU) jisKanaMode.setKanaMode(false);
        if (item.getKey() == KeyCode::JIS_KANA) jisKanaMode.setKanaMode(true);
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
  ModifierCanceling::keyRelease(const RemapParams &params, ModifierFlag::ModifierFlag modifierFlag)
  {
    FlagStatus *flagStatus = allFlagStatus.getFlagStatus(modifierFlag);
    if (flagStatus == NULL) return;

    flagStatus->temporary_decrease();

    if (isSendKeyRelease) return;

    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.add(FireExtraKey::TYPE_BEFORE, KeyEvent::MODIFY, flags, flagStatus->getKeyCode(), 0);

    isSendKeyRelease = true;
  }

  void
  ModifierCanceling::restore(const RemapParams &params, ModifierFlag::ModifierFlag modifierFlag)
  {
    FlagStatus *flagStatus = allFlagStatus.getFlagStatus(modifierFlag);
    if (flagStatus == NULL) return;

    if (! isSendKeyRelease) return;

    isSendKeyRelease = false;

    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.add(FireExtraKey::TYPE_BEFORE, KeyEvent::MODIFY, flags, flagStatus->getKeyCode(), 0);
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
    listFireExtraKey.addKey(0, KeyCode::ESCAPE, CharCode::ESCAPE);
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
    listFireExtraKey.addKey(0, KeyCode::RETURN, CharCode::RETURN);
  }

  void
  FireFunc::firefunc_space(const RemapParams &params)
  {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.addKey(flags, KeyCode::SPACE, CharCode::SPACE);
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

  // ----------------------------------------
  namespace {
    bool
    remapToModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag)
    {
      bool isKeyDown = false;

      if (*(params.eventType) == KeyEvent::MODIFY) {
        // remap Modifier to Modifier

        ModifierFlag::ModifierFlag fromFlag = RemapUtil::getKeyCodeModifier(fromKeyCode);
        FlagStatus *status = allFlagStatus.getFlagStatus(fromFlag);
        if (status == NULL) return false;

        if (status->isHeldDown()) {
          isKeyDown = true;
        }
        RemapUtil::modifierToModifier(params, fromFlag, toFlag);

      } else {
        // remap Key to Modifier

        if (*(params.eventType) == KeyEvent::DOWN) {
          isKeyDown = true;
        }
        RemapUtil::keyToModifier(params, fromKeyCode, toFlag);
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
          firefunc(params);
        }
      }
      clickWatcher.unset(&isClick);
    }
  }

  void
  KeyOverlaidKeyCombination::remap(const RemapParams &params, KeyCode::KeyCode keyCode1, KeyCode::KeyCode keyCode2, FireFunc::FireFunc firefunc)
  {
    if (RemapUtil::isKey(params, keyCode1)) {
      isKey1HeldDown = RemapUtil::isKeyDown(params, keyCode1);
    }

    if (! RemapUtil::isKey(params, keyCode2)) {
      isCallFireFunc = false;
      return;
    }

    if (RemapUtil::isKeyDown(params, keyCode2)) {
      isCallFireFunc = true;
      clickWatcher.set(&isClick);

    } else {
      if (isKey1HeldDown && isCallFireFunc && isClick == false) {
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
        modifierCanceling.keyRelease(params, fromFlag);
        return true;
      }
    }
    modifierCanceling.restore(params, fromFlag);
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
}
