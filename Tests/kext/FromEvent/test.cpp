#include <ostream>
#include <gtest/gtest.h>
#include "FromEvent.hpp"
#include "KeyCodeModifierFlagPairs.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

ParamsUnion* down_return;
ParamsUnion* up_return;
ParamsUnion* down_shift;
ParamsUnion* up_shift;

TEST(Generic, setUp) {
  KeyCodeModifierFlagPairs::clearVirtualModifiers();

  down_return = new ParamsUnion(
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

  up_return = new ParamsUnion(
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

  down_shift = new ParamsUnion(
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

  up_shift = new ParamsUnion(
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
}

TEST(Generic, getModifierFlag) {
  {
    FromEvent fe(KeyCode::RETURN);
    EXPECT_EQ(ModifierFlag::ZERO, fe.getModifierFlag());
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    EXPECT_EQ(ModifierFlag::SHIFT_L, fe.getModifierFlag());
  }
  {
    FromEvent fe(ConsumerKeyCode::VOLUME_MUTE);
    EXPECT_EQ(ModifierFlag::ZERO, fe.getModifierFlag());
  }
  {
    FromEvent fe(PointingButton::LEFT);
    EXPECT_EQ(ModifierFlag::ZERO, fe.getModifierFlag());
  }
}

TEST(Generic, getPointingButton) {
  {
    FromEvent fe(KeyCode::RETURN);
    EXPECT_EQ(PointingButton::NONE, fe.getPointingButton());
  }
  {
    FromEvent fe(ConsumerKeyCode::VOLUME_MUTE);
    EXPECT_EQ(PointingButton::NONE, fe.getPointingButton());
  }
  {
    FromEvent fe(PointingButton::LEFT);
    EXPECT_EQ(PointingButton::LEFT, fe.getPointingButton());
  }
}

TEST(Generic, changePressingState) {
  {
    FromEvent fe(KeyCode::RETURN);
    EXPECT_EQ(false, fe.isPressing());

    // ----------------------------------------
    // Without Flags
    // (For example, "change return to tab".)
    {
      FlagStatus currentFlags;
      Vector_ModifierFlag fromFlags;

      EXPECT_EQ(true,  fe.changePressingState(*down_return, currentFlags, fromFlags));
      EXPECT_EQ(true,  fe.isPressing());

      EXPECT_EQ(true,  fe.changePressingState(*up_return, currentFlags, fromFlags));
      EXPECT_EQ(false, fe.isPressing());

      // Another event does not modify state
      EXPECT_EQ(false, fe.changePressingState(*down_shift, currentFlags, fromFlags));
      EXPECT_EQ(false, fe.isPressing());
      EXPECT_EQ(false, fe.changePressingState(*up_shift, currentFlags, fromFlags));
      EXPECT_EQ(false, fe.isPressing());
    }

    // ----------------------------------------
    // Set currentFlags
    // (For example, "change return to tab".)
    {
      Flags f(ModifierFlag::SHIFT_L);
      FlagStatus currentFlags(f);
      Vector_ModifierFlag fromFlags;

      EXPECT_EQ(true,  fe.changePressingState(*down_return, currentFlags, fromFlags));
      EXPECT_EQ(true,  fe.isPressing());

      EXPECT_EQ(true,  fe.changePressingState(*up_return, currentFlags, fromFlags));
      EXPECT_EQ(false, fe.isPressing());
    }

    // ----------------------------------------
    // Set fromFlags
    // (For example, "change shift-return to tab".)
    {
      FlagStatus currentFlags;
      Vector_ModifierFlag fromFlags;
      fromFlags.push_back(ModifierFlag::SHIFT_L);

      // Does not change state if currentFlags lacks flags.
      EXPECT_EQ(false, fe.changePressingState(*down_return, currentFlags, fromFlags));
      EXPECT_EQ(false, fe.isPressing());

      EXPECT_EQ(false, fe.changePressingState(*up_return, currentFlags, fromFlags));
      EXPECT_EQ(false, fe.isPressing());
    }
    {
      Flags f(ModifierFlag::SHIFT_L);
      FlagStatus currentFlags(f);
      Vector_ModifierFlag fromFlags;
      fromFlags.push_back(ModifierFlag::SHIFT_L);

      EXPECT_EQ(true,  fe.changePressingState(*down_return, currentFlags, fromFlags));
      EXPECT_EQ(true,  fe.isPressing());

      EXPECT_EQ(true,  fe.changePressingState(*up_return, currentFlags, fromFlags));
      EXPECT_EQ(false, fe.isPressing());
    }
    {
      Flags f(ModifierFlag::SHIFT_L);
      FlagStatus currentFlags(f);
      Vector_ModifierFlag fromFlags;
      fromFlags.push_back(ModifierFlag::SHIFT_L);

      EXPECT_EQ(true,  fe.changePressingState(*down_return, currentFlags, fromFlags));
      EXPECT_EQ(true,  fe.isPressing());

      // Change state even if currentFlags lacks flags when key is pressing.
      // This behavior is necessary for this case.
      // - shift down
      // - return down (shift-return is pressed.)
      // - shift up
      // - return up (shift-return is released.)
      currentFlags.reset();
      EXPECT_EQ(true,  fe.changePressingState(*up_return, currentFlags, fromFlags));
      EXPECT_EQ(false, fe.isPressing());
      // return false if call changePressingState once again.
      EXPECT_EQ(false, fe.changePressingState(*up_return, currentFlags, fromFlags));
    }
  }
  {
    FromEvent fe(KeyCode::SPACE);
    FlagStatus currentFlags;
    Vector_ModifierFlag fromFlags;
    EXPECT_EQ(false, fe.changePressingState(*down_return, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.changePressingState(*up_return, currentFlags, fromFlags));
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    Flags f(ModifierFlag::SHIFT_L);
    FlagStatus currentFlags(f);
    Vector_ModifierFlag fromFlags;
    EXPECT_EQ(true, fe.changePressingState(*down_shift, currentFlags, fromFlags));
    EXPECT_EQ(true, fe.isPressing());

    currentFlags.reset();
    EXPECT_EQ(true,  fe.changePressingState(*up_shift, currentFlags, fromFlags));
    EXPECT_EQ(false, fe.isPressing());
  }
}

TEST(Generic, unsetPressingState) {
  {
    FromEvent fe(KeyCode::RETURN);
    FlagStatus currentFlags;
    Vector_ModifierFlag fromFlags;

    EXPECT_EQ(true, fe.changePressingState(*down_return, currentFlags, fromFlags));
    EXPECT_EQ(true, fe.isPressing());

    fe.unsetPressingState();
    EXPECT_EQ(false, fe.isPressing());
  }
}

TEST(Generic, isTargetDownEvent) {
  {
    FromEvent fe(KeyCode::RETURN);
    EXPECT_EQ(true,  fe.isTargetDownEvent(*down_return));
    EXPECT_EQ(false, fe.isTargetUpEvent(*down_return));

    EXPECT_EQ(false, fe.isTargetDownEvent(*up_return));
    EXPECT_EQ(true,  fe.isTargetUpEvent(*up_return));

    EXPECT_EQ(false, fe.isTargetDownEvent(*down_shift));
    EXPECT_EQ(false, fe.isTargetUpEvent(*down_shift));
    EXPECT_EQ(false, fe.isTargetDownEvent(*up_shift));
    EXPECT_EQ(false, fe.isTargetUpEvent(*up_shift));
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    EXPECT_EQ(true,  fe.isTargetDownEvent(*down_shift));
    EXPECT_EQ(false, fe.isTargetUpEvent(*down_shift));

    EXPECT_EQ(false, fe.isTargetDownEvent(*up_shift));
    EXPECT_EQ(true,  fe.isTargetUpEvent(*up_shift));
  }
}
