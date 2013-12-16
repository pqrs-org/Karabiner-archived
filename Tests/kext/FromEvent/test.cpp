#include <ostream>
#include <gtest/gtest.h>
#include "FromEvent.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

ParamsUnion down_return(
  *(Params_KeyboardEventCallBack::auto_ptr(
      Params_KeyboardEventCallBack::alloc(
        EventType::DOWN,
        Flags(0),
        KeyCode::RETURN,
        CharCode(0),
        CharSet(0),
        OrigCharCode(0),
        OrigCharSet(0),
        KeyboardType(0),
        false))));

ParamsUnion up_return(
  *(Params_KeyboardEventCallBack::auto_ptr(
      Params_KeyboardEventCallBack::alloc(
        EventType::UP,
        Flags(0),
        KeyCode::RETURN,
        CharCode(0),
        CharSet(0),
        OrigCharCode(0),
        OrigCharSet(0),
        KeyboardType(0),
        false))));

ParamsUnion down_shift(
  *(Params_KeyboardEventCallBack::auto_ptr(
      Params_KeyboardEventCallBack::alloc(
        EventType::MODIFY,
        Flags(ModifierFlag::SHIFT_L),
        KeyCode::SHIFT_L,
        CharCode(0),
        CharSet(0),
        OrigCharCode(0),
        OrigCharSet(0),
        KeyboardType(0),
        false))));

ParamsUnion up_shift(
  *(Params_KeyboardEventCallBack::auto_ptr(
      Params_KeyboardEventCallBack::alloc(
        EventType::MODIFY,
        Flags(0),
        KeyCode::SHIFT_L,
        CharCode(0),
        CharSet(0),
        OrigCharCode(0),
        OrigCharSet(0),
        KeyboardType(0),
        false))));


TEST(Generic, getModifierFlag) {
  {
    FromEvent fe(KeyCode::RETURN);
    EXPECT_EQ(ModifierFlag::NONE, fe.getModifierFlag());
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    EXPECT_EQ(ModifierFlag::SHIFT_L, fe.getModifierFlag());
  }
  {
    FromEvent fe(ConsumerKeyCode::VOLUME_MUTE);
    EXPECT_EQ(ModifierFlag::NONE, fe.getModifierFlag());
  }
  {
    FromEvent fe(PointingButton::LEFT);
    EXPECT_EQ(ModifierFlag::NONE, fe.getModifierFlag());
  }
}

TEST(Generic, changePressingState) {
  {
    FromEvent fe(KeyCode::RETURN);
    EXPECT_EQ(false, fe.isPressing());

    Flags currentFlags(0);
    Flags fromFlags(0);

    // ----------------------------------------
    // Without Flags
    // (For example, "change return to tab".)

    EXPECT_EQ(true,  fe.changePressingState(down_return, currentFlags, fromFlags));
    EXPECT_EQ(true,  fe.isPressing());

    EXPECT_EQ(true,  fe.changePressingState(up_return, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());

    // Another event does not modify state
    EXPECT_EQ(false, fe.changePressingState(down_shift, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());
    EXPECT_EQ(false, fe.changePressingState(up_shift, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());

    // ----------------------------------------
    // Set currentFlags
    // (For example, "change return to tab".)

    currentFlags = Flags(ModifierFlag::SHIFT_L);
    EXPECT_EQ(true,  fe.changePressingState(down_return, currentFlags, fromFlags));
    EXPECT_EQ(true,  fe.isPressing());

    EXPECT_EQ(true,  fe.changePressingState(up_return, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());

    // ----------------------------------------
    // Set fromFlags
    // (For example, "change shift-return to tab".)

    // Does not change state if currentFlags lacks flags.
    currentFlags = Flags(0);
    fromFlags = Flags(ModifierFlag::SHIFT_L);
    EXPECT_EQ(false, fe.changePressingState(down_return, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());

    EXPECT_EQ(false, fe.changePressingState(up_return, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());

    // ----------
    currentFlags = Flags(ModifierFlag::SHIFT_L);
    fromFlags = Flags(ModifierFlag::SHIFT_L);
    EXPECT_EQ(true,  fe.changePressingState(down_return, currentFlags, fromFlags));
    EXPECT_EQ(true,  fe.isPressing());

    EXPECT_EQ(true,  fe.changePressingState(up_return, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());

    // ----------
    currentFlags = Flags(ModifierFlag::SHIFT_L);
    fromFlags = Flags(ModifierFlag::SHIFT_L);
    EXPECT_EQ(true,  fe.changePressingState(down_return, currentFlags, fromFlags));
    EXPECT_EQ(true,  fe.isPressing());

    // Change state even if currentFlags lacks flags when key is pressing.
    // This behavior is necessary for this case.
    // - shift down
    // - return down (shift-return is pressed.)
    // - shift up
    // - return up (shift-return is released.)
    currentFlags = Flags(0);
    EXPECT_EQ(true,  fe.changePressingState(up_return, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());
    // return false if call changePressingState once again.
    EXPECT_EQ(false, fe.changePressingState(up_return, currentFlags, fromFlags));
  }
  {
    FromEvent fe(KeyCode::SPACE);
    EXPECT_EQ(false, fe.changePressingState(down_return, Flags(0), Flags(0)));
    EXPECT_EQ(false, fe.changePressingState(up_return, Flags(0), Flags(0)));
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    EXPECT_EQ(true,  fe.changePressingState(down_shift, Flags(ModifierFlag::SHIFT_L), Flags(0)));
    EXPECT_EQ(true,  fe.isPressing());

    EXPECT_EQ(true,  fe.changePressingState(up_shift, Flags(0), Flags(0)));
    EXPECT_EQ(false, fe.isPressing());
  }
}

TEST(Generic, unsetPressingState) {
  {
    FromEvent fe(KeyCode::RETURN);

    EXPECT_EQ(true,  fe.changePressingState(down_return, Flags(0), Flags(0)));
    EXPECT_EQ(true,  fe.isPressing());

    fe.unsetPressingState();
    EXPECT_EQ(false, fe.isPressing());
  }
}

TEST(Generic, isTargetDownEvent) {
  {
    FromEvent fe(KeyCode::RETURN);
    EXPECT_EQ(true,  fe.isTargetDownEvent(down_return));
    EXPECT_EQ(false, fe.isTargetUpEvent(down_return));

    EXPECT_EQ(false, fe.isTargetDownEvent(up_return));
    EXPECT_EQ(true,  fe.isTargetUpEvent(up_return));

    EXPECT_EQ(false, fe.isTargetDownEvent(down_shift));
    EXPECT_EQ(false, fe.isTargetUpEvent(down_shift));
    EXPECT_EQ(false, fe.isTargetDownEvent(up_shift));
    EXPECT_EQ(false, fe.isTargetUpEvent(up_shift));
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    EXPECT_EQ(true,  fe.isTargetDownEvent(down_shift));
    EXPECT_EQ(false, fe.isTargetUpEvent(down_shift));

    EXPECT_EQ(false, fe.isTargetDownEvent(up_shift));
    EXPECT_EQ(true,  fe.isTargetUpEvent(up_shift));
  }
}
