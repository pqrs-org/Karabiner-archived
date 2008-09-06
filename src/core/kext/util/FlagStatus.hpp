#ifndef FLAGSTATUS_HPP
#define FLAGSTATUS_HPP

#include "base.hpp"
#include "keycode.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class FlagStatus {
  public:
    FlagStatus(ModifierFlag::ModifierFlag _flag);
    void initialize(const RemapParams &params);

    void reset(void) { count = 0; }
    void increase(void) { ++count; }
    void decrease(void) { --count; }
    void temporary_increase(void) { ++temporary_count; }
    void temporary_decrease(void) { --temporary_count; }

    bool isHeldDown(void) const { return (count + temporary_count) > 0; }
    unsigned int makeFlag(void) const { return (isHeldDown()) ? flag : 0; }

    KeyCode::KeyCode getKeyCode(void) const { return key; }

  private:
    ModifierFlag::ModifierFlag flag;
    KeyCode::KeyCode key;
    int count;
    int temporary_count;
  };

  class AllFlagStatus {
  public:
    FlagStatus capslock;
    FlagStatus shiftL;
    FlagStatus shiftR;
    FlagStatus controlL;
    FlagStatus controlR;
    FlagStatus optionL;
    FlagStatus optionR;
    FlagStatus commandL;
    FlagStatus commandR;
    FlagStatus fn;

    int numHeldDownKeys;

    AllFlagStatus(void) :
      capslock(ModifierFlag::CAPSLOCK),
      shiftL(ModifierFlag::SHIFT_L),
      shiftR(ModifierFlag::SHIFT_R),
      controlL(ModifierFlag::CONTROL_L),
      controlR(ModifierFlag::CONTROL_R),
      optionL(ModifierFlag::OPTION_L),
      optionR(ModifierFlag::OPTION_R),
      commandL(ModifierFlag::COMMAND_L),
      commandR(ModifierFlag::COMMAND_R),
      fn(ModifierFlag::FN)
    {}
    void initialize(const RemapParams &params);
    unsigned int makeFlags(unsigned int keyCode);
    unsigned int makeFlags(const RemapParams &params);
    void reset(void);

    FlagStatus *getFlagStatus(ModifierFlag::ModifierFlag flag);
    FlagStatus *getFlagStatus(KeyCode::KeyCode keyCode);

    // ----------------------------------------
    bool isHeldDown_command(void) { return commandL.isHeldDown() || commandR.isHeldDown(); }
    bool isHeldDown_control(void) { return controlL.isHeldDown() || controlR.isHeldDown(); }
    bool isHeldDown_option(void) { return optionL.isHeldDown() || optionR.isHeldDown(); }
    bool isHeldDown_shift(void) { return shiftL.isHeldDown() || shiftR.isHeldDown(); }

    void temporaryDecrease_control(void) {
      if (controlL.isHeldDown()) {
        controlL.temporary_decrease();
      } else {
        controlR.temporary_decrease();
      }
    }
    void temporaryDecrease_command(void) {
      if (commandL.isHeldDown()) {
        commandL.temporary_decrease();
      } else {
        commandR.temporary_decrease();
      }
    }
    void temporaryDecrease_option(void) {
      if (optionL.isHeldDown()) {
        optionL.temporary_decrease();
      } else {
        optionR.temporary_decrease();
      }
    }
    void temporaryDecrease_shift(void) {
      if (shiftL.isHeldDown()) {
        shiftL.temporary_decrease();
      } else {
        shiftR.temporary_decrease();
      }
    }
  };
}

#endif
