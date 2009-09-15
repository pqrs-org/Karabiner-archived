#ifndef FLAGSTATUS_HPP
#define FLAGSTATUS_HPP

#include "base.hpp"
#include "keycode.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace FlagStatus {
    class Item {
    public:
      void initialize(ModifierFlag::ModifierFlag _flag);
      void set(const RemapParams &remapParams);

      void reset(void);
      void increase(void);
      void decrease(void);
      void temporary_increase(void) { ++temporary_count; }
      void temporary_decrease(void) { --temporary_count; }
      void lock_increase(void) { lock_count = 1; }
      void lock_decrease(void) { lock_count = 0; }

      bool isHeldDown(void) const { return (count + temporary_count + lock_count + original_lock_count) > 0; }
      unsigned int makeFlag(void) const { return (isHeldDown()) ? flag : 0; }

      KeyCode::KeyCode getKeyCode(void) const { return key; }

    private:
      ModifierFlag::ModifierFlag flag;
      KeyCode::KeyCode key;
      int count;
      int temporary_count;

      int lock_count; // store remapped lock status. (CapsLock, FN lock, ...)
      int original_lock_count; // store original CapsLock status.
    };

    void initialize(void);
    void set(const RemapParams &remapParams);
    unsigned int makeFlags(unsigned int keyCode = KeyCode::NONE);
    unsigned int makeFlags(const RemapParams &remapParams);
    void reset(void);

    Item *getFlagStatus(ModifierFlag::ModifierFlag flag);
    Item *getFlagStatus(KeyCode::KeyCode keyCode);

    bool isHeldDown(ModifierFlag::ModifierFlag flag);
    void increase(ModifierFlag::ModifierFlag flag);
    void decrease(ModifierFlag::ModifierFlag flag);
    void temporary_increase(ModifierFlag::ModifierFlag flag);
    void temporary_decrease(ModifierFlag::ModifierFlag flag);
    void lock_increase(ModifierFlag::ModifierFlag flag);
    void lock_decrease(ModifierFlag::ModifierFlag flag);

    // ----------------------------------------
    inline bool isHeldDown_command(void) { return isHeldDown(ModifierFlag::COMMAND_L) || isHeldDown(ModifierFlag::COMMAND_R); }
    inline bool isHeldDown_control(void) { return isHeldDown(ModifierFlag::CONTROL_L) || isHeldDown(ModifierFlag::CONTROL_R); }
    inline bool isHeldDown_option(void) { return isHeldDown(ModifierFlag::OPTION_L) || isHeldDown(ModifierFlag::OPTION_R); }
    inline bool isHeldDown_shift(void) { return isHeldDown(ModifierFlag::SHIFT_L) || isHeldDown(ModifierFlag::SHIFT_R); }

    inline void temporaryDecrease_control(void) {
      if (isHeldDown(ModifierFlag::CONTROL_L)) {
        temporary_decrease(ModifierFlag::CONTROL_L);
      } else {
        temporary_decrease(ModifierFlag::CONTROL_R);
      }
    }
    inline void temporaryDecrease_command(void) {
      if (isHeldDown(ModifierFlag::COMMAND_L)) {
        temporary_decrease(ModifierFlag::COMMAND_L);
      } else {
        temporary_decrease(ModifierFlag::COMMAND_R);
      }
    }
    inline void temporaryDecrease_option(void) {
      if (isHeldDown(ModifierFlag::OPTION_L)) {
        temporary_decrease(ModifierFlag::OPTION_L);
      } else {
        temporary_decrease(ModifierFlag::OPTION_R);
      }
    }
    inline void temporaryDecrease_shift(void) {
      if (isHeldDown(ModifierFlag::SHIFT_L)) {
        temporary_decrease(ModifierFlag::SHIFT_L);
      } else {
        temporary_decrease(ModifierFlag::SHIFT_R);
      }
    }
  }
}

#endif
