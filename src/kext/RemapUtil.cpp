#include <sys/systm.h>

#include "RemapUtil.hpp"
#include "keycode.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  AllFlagStatus allFlagStatus;
  ListFireExtraKey listFireExtraKey;
  ListFirePointingClick listFirePointingClick;
  FirePointingScroll firePointingScroll;
  ClickWatcher clickWatcher;
  PointingButtonStatus pointingButtonStatus;

  bool
  RemapUtil::isModifierOn(const RemapParams &params, ModifierFlag::ModifierFlag flag)
  {
    return ((*(params.flags) & flag) == flag);
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
    if (*(params.key) == KeyCode::CURSOR_UP) {
      *(params.key) = KeyCode::PAGEUP;
    } else if (*(params.key) == KeyCode::CURSOR_DOWN) {
      *(params.key) = KeyCode::PAGEDOWN;
    } else if (*(params.key) == KeyCode::CURSOR_LEFT) {
      *(params.key) = KeyCode::HOME;
    } else if (*(params.key) == KeyCode::CURSOR_RIGHT) {
      *(params.key) = KeyCode::END;
    } else if (*(params.key) == KeyCode::RETURN) {
      *(params.key) = getEnterKeyCode(params);
    } else if (*(params.key) == KeyCode::DELETE) {
      *(params.key) = KeyCode::FORWARD_DELETE;
    } else {
      return;
    }
    // strip CURSOR flag
    allFlagStatus.cursor = false;
  }

  void
  RemapUtil::toDelete(const RemapParams &params) {
    if (! allFlagStatus.fn.isHeldDown()) return;

    if (*(params.key) == KeyCode::DELETE) {
      *(params.key) = KeyCode::FORWARD_DELETE;
    }
  }

  KeyCode::KeyCode
  RemapUtil::getEnterKeyCode(const RemapParams &params)
  {
    if (*(params.keyboardType) == KeyboardType::POWERBOOK ||
        *(params.keyboardType) == KeyboardType::POWERBOOK_G4) {
      return KeyCode::ENTER_POWERBOOK;
    }
    return KeyCode::ENTER;
  }

  bool
  RemapUtil::isInternalKeyboard(unsigned int keyboardType) {
    if (keyboardType == KeyboardType::MACBOOK) return true;
    if (keyboardType == KeyboardType::MACBOOK_COREDUO) return true;
    if (keyboardType == KeyboardType::POWERBOOK) return true;
    if (keyboardType == KeyboardType::POWERBOOK_G4) return true;
    return false;
  }

  void
  RemapUtil::modifierToModifier(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, ModifierFlag::ModifierFlag toFlag)
  {
    KeyCode::KeyCode fromKeyCode = getModifierKeyCode(fromFlag);
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;

    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    if (fromStatus == NULL) return;
    FlagStatus *toStatus = allFlagStatus.getFlagStatus(toFlag);
    if (toStatus == NULL) return;

    if (isModifierOn(params, fromFlag)) {
      toStatus->increase();
      fromStatus->decrease();
    } else {
      toStatus->decrease();
      fromStatus->increase();
    }

    KeyCode::KeyCode toKeyCode = getModifierKeyCode(toFlag);
    *(params.key) = toKeyCode;
  }

  void
  RemapUtil::modifierToKey(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, KeyCode::KeyCode toKeyCode) {
    KeyCode::KeyCode fromKeyCode = getModifierKeyCode(fromFlag);
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;

    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    if (fromStatus == NULL) return;

    if (isModifierOn(params, fromFlag)) {
      fromStatus->decrease();
      *(params.eventType) = KeyEvent::DOWN;
    } else {
      fromStatus->increase();
      *(params.eventType) = KeyEvent::UP;
    }

    *(params.key) = toKeyCode;
  }

  void
  RemapUtil::keyToModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag) {
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;

    FlagStatus *toStatus = allFlagStatus.getFlagStatus(toFlag);
    if (toStatus == NULL) return;

    if (*(params.eventType) == KeyEvent::DOWN) {
      toStatus->increase();
    } else if (*(params.eventType) == KeyEvent::UP) {
      toStatus->decrease();
    }

    KeyCode::KeyCode toKeyCode = getModifierKeyCode(toFlag);
    *(params.key) = toKeyCode;
    *(params.eventType) = KeyEvent::MODIFY;
  }

  void
  RemapUtil::keyToModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag1, ModifierFlag::ModifierFlag toFlag2) {
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;

    FlagStatus *toStatus1 = allFlagStatus.getFlagStatus(toFlag1);
    FlagStatus *toStatus2 = allFlagStatus.getFlagStatus(toFlag2);
    if (toStatus1 == NULL || toStatus2 == NULL) return;

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
  }

  void
  RemapUtil::keyToKey(const RemapParams &params, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode) {
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;
    *(params.key) = toKeyCode;
  }

  // ----------
  void
  RemapUtil::modifierToPointingButton(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, PointingButton::PointingButton toButton) {
    KeyCode::KeyCode fromKeyCode = getModifierKeyCode(fromFlag);
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;

    FlagStatus *fromStatus = allFlagStatus.getFlagStatus(fromFlag);
    if (fromStatus == NULL) return;

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

    *(params.ex_dropKey) = true;
  }

  void
  RemapUtil::keyToPointingButton(const RemapParams &params, KeyCode::KeyCode fromKeyCode, PointingButton::PointingButton toButton) {
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;

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

  // --------------------
  bool
  RemapUtil::al2number(const RemapParams &params) {
    unsigned int origKey = *(params.key);
    keyToKey(params, KeyCode::A, KeyCode::KEY_1);
    keyToKey(params, KeyCode::S, KeyCode::KEY_2);
    keyToKey(params, KeyCode::D, KeyCode::KEY_3);
    keyToKey(params, KeyCode::F, KeyCode::KEY_4);
    keyToKey(params, KeyCode::G, KeyCode::KEY_5);
    keyToKey(params, KeyCode::H, KeyCode::KEY_6);
    keyToKey(params, KeyCode::J, KeyCode::KEY_7);
    keyToKey(params, KeyCode::K, KeyCode::KEY_8);
    keyToKey(params, KeyCode::L, KeyCode::KEY_9);

    return *(params.key) != origKey;
  }

  bool
  RemapUtil::keypad2spaces(const RemapParams &params) {
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
  RemapUtil::jis_toggle_eisuu_kana(const RemapParams &params, KeyCode::KeyCode fromKeyCode) {
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;

    static bool isKana = true;

    if (isKana) {
      RemapUtil::keyToKey(params, fromKeyCode, KeyCode::JIS_KANA);
    } else {
      RemapUtil::keyToKey(params, fromKeyCode, KeyCode::JIS_EISUU);
    }
    if (*(params.eventType) == KeyEvent::UP) {
      isKana = ! isKana;
    }
  }

  void
  RemapUtil::jis_toggle_eisuu_kana(const RemapParams &params, ModifierFlag::ModifierFlag fromModifier) {
    KeyCode::KeyCode fromKeyCode = getModifierKeyCode(fromModifier);

    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) return;

    static bool isKana = true;

    if (isKana) {
      RemapUtil::modifierToKey(params, fromModifier, KeyCode::JIS_KANA);
    } else {
      RemapUtil::modifierToKey(params, fromModifier, KeyCode::JIS_EISUU);
    }
    if (*(params.eventType) == KeyEvent::UP) {
      isKana = ! isKana;
    }
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

    int delta1 = - *(params.dy);
    int delta2 = - *(params.dx);

    if (config.option_pointing_disable_vertical_scroll) delta1 = 0;
    if (config.option_pointing_disable_horizontal_scroll) delta2 = 0;

    // ----------------------------------------
    int deltaAxis1 = (delta1 * config.pointing_relative2scroll_rate) / 1000;
    if (deltaAxis1 == 0) {
      if (*(params.dy) < 0) deltaAxis1 = 1;
      if (*(params.dy) > 0) deltaAxis1 = -1;
    }
    int deltaAxis2 = (delta2 * config.pointing_relative2scroll_rate) / 1000;
    if (deltaAxis2 == 0) {
      if (*(params.dx) < 0) deltaAxis2 = 1;
      if (*(params.dx) > 0) deltaAxis2 = -1;
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

  // ----------------------------------------------------------------------
  void
  ListFireExtraKey::reset(void)
  {
    for (int i = 0; i < FIREEXTRAKEY_MAXNUM; ++i) {
      list[i].unset();
    }
  }

  void
  ListFireExtraKey::add(FireExtraKey::Type type, unsigned int eventType, unsigned int flags, unsigned int key, unsigned int charCode)
  {
    for (int i = 0; i < FIREEXTRAKEY_MAXNUM; ++i) {
      if (! list[i].isEnable()) {
        list[i].set(type, eventType, flags, key, charCode);
        break;
      }
    }
  }

  void
  ListFireExtraKey::fire(FireExtraKey::Type type, KeyboardEventCallback callback, OSObject *target, OSObject *sender, void *refcon, const RemapParams &params)
  {
    if (callback == NULL) return;

    for (int i = 0; i < FIREEXTRAKEY_MAXNUM; ++i) {
      FireExtraKey &item = list[i];

      if (item.isEnable() && item.getType() == type) {
        callback(target, item.getEventType(), item.getFlags(), item.getKey(), item.getCharCode(),
                 *(params.charSet), *(params.origCharCode), *(params.origCharSet),
                 *(params.keyboardType), false, *(params.ts), sender, refcon);
      }
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
  FireFunc::firefunc_commandSpace(const RemapParams &params) {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, ModifierFlag::COMMAND_L, KeyCode::COMMAND_L, CharCode::COMMAND_L);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN,   ModifierFlag::COMMAND_L, KeyCode::SPACE,     CharCode::SPACE);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP,     ModifierFlag::COMMAND_L, KeyCode::SPACE,     CharCode::SPACE);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::MODIFY, 0,                       KeyCode::COMMAND_L, CharCode::COMMAND_L);
  }

  void
  FireFunc::firefunc_escape(const RemapParams &params) {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, flags, KeyCode::ESCAPE, CharCode::ESCAPE);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, flags, KeyCode::ESCAPE, CharCode::ESCAPE);
  }

  void
  FireFunc::firefunc_space(const RemapParams &params) {
    unsigned int flags = allFlagStatus.makeFlags(params);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, flags, KeyCode::SPACE, CharCode::SPACE);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, flags, KeyCode::SPACE, CharCode::SPACE);
  }

  void
  FireFunc::firefunc_jis_kana(const RemapParams &params) {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, 0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, 0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
  }

  void
  FireFunc::firefunc_jis_kana_x2(const RemapParams &params) {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, 0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, 0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, 0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, 0, KeyCode::JIS_KANA, CharCode::JIS_KANA);
  }

  void
  FireFunc::firefunc_jis_eisuu(const RemapParams &params) {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, 0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, 0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
  }

  void
  FireFunc::firefunc_jis_eisuu_x2(const RemapParams &params) {
    // fire only if no-modifiers
    if (allFlagStatus.makeFlags(params) != 0) return;

    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, 0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, 0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::DOWN, 0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
    listFireExtraKey.add(FireExtraKey::TYPE_AFTER, KeyEvent::UP, 0, KeyCode::JIS_EISUU, CharCode::JIS_EISUU);
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
  KeyOverlayedModifier::remap(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, FireFunc::FireFunc firefunc)
  {
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) {
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
    } else {
      if (useAsModifier == false && isClick == false) {
        firefunc(params);
      }
      clickWatcher.unset(&isClick);
    }
  }

  void
  DoublePressModifier::remap(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, FireFunc::FireFunc firefunc)
  {
    if (params.ex_origKey != fromKeyCode || *(params.key) != fromKeyCode) {
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
  ListFirePointingClick::reset(void)
  {
    for (int i = 0; i < FIREPOINTINGCLICK_MAXNUM; ++i) {
      list[i].unset();
    }
  }

  bool
  ListFirePointingClick::isEmpty(void)
  {
    for (int i = 0; i < FIREPOINTINGCLICK_MAXNUM; ++i) {
      if (list[i].isEnable()) return false;
    }
    return true;
  }

  void
  ListFirePointingClick::add(PointingButton::PointingButton button)
  {
    for (int i = 0; i < FIREPOINTINGCLICK_MAXNUM; ++i) {
      if (! list[i].isEnable()) {
        list[i].set(button);
        break;
      }
    }
  }

  void
  ListFirePointingClick::fire(RelativePointerEventCallback callback, OSObject *target, OSObject *sender, AbsoluteTime ts)
  {
    if (callback == NULL) return;

    for (int i = 0; i < FIREPOINTINGCLICK_MAXNUM; ++i) {
      FirePointingClick &item = list[i];

      if (item.isEnable()) {
        callback(target, item.getButton(), 0, 0, ts, sender, 0);
      }
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
  }
}
