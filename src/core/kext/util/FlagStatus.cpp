#include "FlagStatus.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace FlagStatus {
    Item item[ModifierFlag::listsize];

    void
    Item::initialize(ModifierFlag::ModifierFlag _flag)
    {
      flag = _flag;
      key = RemapUtil::getModifierKeyCode(_flag);
      count = 0;
      temporary_count = 0;
      lock_count = 0;
    }

    void
    Item::set(const RemapParams &remapParams)
    {
      temporary_count = 0;

      if ((remapParams.params)->eventType != KeyEvent::MODIFY) return;
      if (remapParams.ex_origKey != static_cast<unsigned int>(key)) return;

      if (RemapUtil::isModifierOn(remapParams, flag)) {
        increase();
      } else {
        decrease();
      }
    }

    // ----------------------------------------------------------------------
    void
    initialize(void)
    {
      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        item[i].initialize(ModifierFlag::list[i]);
      }
    }

    void
    set(const RemapParams &remapParams)
    {
      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        item[i].set(remapParams);
      }
    }

    void
    reset(void)
    {
      if (config.debug_devel) {
        printf("KeyRemap4MacBook FlagStatus::reset\n");
      }

      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        item[i].reset();
      }
    }

    void
    reset_lock(void)
    {
      if (config.debug_devel) {
        printf("KeyRemap4MacBook FlagStatus::reset_lock\n");
      }

      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        item[i].reset_lock();
      }
    }

    unsigned int
    makeFlags(unsigned int keyCode)
    {
      unsigned int flags = 0;
      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        flags |= item[i].makeFlag();
      }

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

      if (config.debug_devel) {
        printf("KeyRemap4MacBook -INFO- FlagStatus::makeFlags = 0x%x\n", flags);
      }

      return flags;
    }

    unsigned int
    makeFlags(const RemapParams &remapParams)
    {
      return makeFlags((remapParams.params)->key);
    }

    Item *
    getFlagStatus(ModifierFlag::ModifierFlag flag) {
      for (int i = 0; i < ModifierFlag::listsize; ++i) {
        if (flag == ModifierFlag::list[i]) return item + i;
      }
      return NULL;
    }

    // ----------------------------------------
    bool
    isHeldDown(ModifierFlag::ModifierFlag flag) {
      Item *p = getFlagStatus(flag);
      if (! p) return false;
      return p->isHeldDown();
    }

    void
    increase(ModifierFlag::ModifierFlag flag) {
      Item *p = getFlagStatus(flag);
      if (! p) return;
      return p->increase();
    }

    void
    decrease(ModifierFlag::ModifierFlag flag) {
      Item *p = getFlagStatus(flag);
      if (! p) return;
      return p->decrease();
    }

    void
    temporary_increase(ModifierFlag::ModifierFlag flag) {
      Item *p = getFlagStatus(flag);
      if (! p) return;
      return p->temporary_increase();
    }

    void
    temporary_decrease(ModifierFlag::ModifierFlag flag) {
      Item *p = getFlagStatus(flag);
      if (! p) return;
      return p->temporary_decrease();
    }

    void
    lock_increase(ModifierFlag::ModifierFlag flag) {
      Item *p = getFlagStatus(flag);
      if (! p) return;
      return p->lock_increase();
    }

    void
    lock_decrease(ModifierFlag::ModifierFlag flag) {
      Item *p = getFlagStatus(flag);
      if (! p) return;
      return p->lock_decrease();
    }
  }
}
