#include "FlagStatus.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  FlagStatus::FlagStatus(ModifierFlag::ModifierFlag _flag)
  {
    flag = _flag;
    key = RemapUtil::getModifierKeyCode(_flag);
    count = 0;
    temporary_count = 0;
  }

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

  // ----------------------------------------------------------------------
  void
  AllFlagStatus::initialize(const RemapParams &params)
  {
    if (RemapUtil::isKeyDown(params, *(params.key))) {
      ++numHeldDownKeys;
    } else {
      --numHeldDownKeys;
    }

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
}
