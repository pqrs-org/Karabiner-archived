#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>

#include "FromEvent.hpp"
#include "KeyCodeModifierFlagPairs.hpp"

using namespace org_pqrs_Karabiner;

Params_KeyboardEventCallBack* down_return;
Params_KeyboardEventCallBack* up_return;
Params_KeyboardEventCallBack* down_shift;
Params_KeyboardEventCallBack* up_shift;

TEST_CASE("setUp", "[Generic]") {
  KeyCodeModifierFlagPairs::clearVirtualModifiers();

  down_return = new Params_KeyboardEventCallBack(
      EventType::DOWN,
      Flags(0),
      KeyCode::RETURN,
      CharCode(0),
      CharSet(0),
      OrigCharCode(0),
      OrigCharSet(0),
      KeyboardType(0),
      false);

  up_return = new Params_KeyboardEventCallBack(
      EventType::UP,
      Flags(0),
      KeyCode::RETURN,
      CharCode(0),
      CharSet(0),
      OrigCharCode(0),
      OrigCharSet(0),
      KeyboardType(0),
      false);

  down_shift = new Params_KeyboardEventCallBack(
      EventType::MODIFY,
      Flags(ModifierFlag::SHIFT_L),
      KeyCode::SHIFT_L,
      CharCode(0),
      CharSet(0),
      OrigCharCode(0),
      OrigCharSet(0),
      KeyboardType(0),
      false);

  up_shift = new Params_KeyboardEventCallBack(
      EventType::MODIFY,
      Flags(0),
      KeyCode::SHIFT_L,
      CharCode(0),
      CharSet(0),
      OrigCharCode(0),
      OrigCharSet(0),
      KeyboardType(0),
      false);
}

TEST_CASE("getModifierFlag", "[Generic]") {
  {
    FromEvent fe(KeyCode::RETURN);
    REQUIRE(fe.getModifierFlag() == ModifierFlag::ZERO);
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    REQUIRE(fe.getModifierFlag() == ModifierFlag::SHIFT_L);
  }
  {
    FromEvent fe(ConsumerKeyCode::VOLUME_MUTE);
    REQUIRE(fe.getModifierFlag() == ModifierFlag::ZERO);
  }
  {
    FromEvent fe(PointingButton::LEFT);
    REQUIRE(fe.getModifierFlag() == ModifierFlag::ZERO);
  }
}

TEST_CASE("getPointingButton", "[Generic]") {
  {
    FromEvent fe(KeyCode::RETURN);
    REQUIRE(fe.getPointingButton() == PointingButton::NONE);
  }
  {
    FromEvent fe(ConsumerKeyCode::VOLUME_MUTE);
    REQUIRE(fe.getPointingButton() == PointingButton::NONE);
  }
  {
    FromEvent fe(PointingButton::LEFT);
    REQUIRE(fe.getPointingButton() == PointingButton::LEFT);
  }
}

TEST_CASE("changePressingState", "[Generic]") {
  {
    FromEvent fe(KeyCode::RETURN);
    REQUIRE(fe.isPressing() == false);

    // ----------------------------------------
    // Without Flags
    // (For example, "change return to tab".)
    {
      FlagStatus currentFlags;
      Vector_ModifierFlag fromFlags;

      REQUIRE(fe.changePressingState(*down_return, currentFlags, fromFlags) == true);
      REQUIRE(fe.isPressing() == true);

      REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == true);
      REQUIRE(fe.isPressing() == false);

      // Another event does not modify state
      REQUIRE(fe.changePressingState(*down_shift, currentFlags, fromFlags) == false);
      REQUIRE(fe.isPressing() == false);
      REQUIRE(fe.changePressingState(*up_shift, currentFlags, fromFlags) == false);
      REQUIRE(fe.isPressing() == false);
    }

    // ----------------------------------------
    // Set currentFlags
    // (For example, "change return to tab".)
    {
      Flags f(ModifierFlag::SHIFT_L);
      FlagStatus currentFlags(f);
      Vector_ModifierFlag fromFlags;

      REQUIRE(fe.changePressingState(*down_return, currentFlags, fromFlags) == true);
      REQUIRE(fe.isPressing() == true);

      REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == true);
      REQUIRE(fe.isPressing() == false);
    }

    // ----------------------------------------
    // Set fromFlags
    // (For example, "change shift-return to tab".)
    {
      FlagStatus currentFlags;
      Vector_ModifierFlag fromFlags;
      fromFlags.push_back(ModifierFlag::SHIFT_L);

      // Does not change state if currentFlags lacks flags.
      REQUIRE(fe.changePressingState(*down_return, currentFlags, fromFlags) == false);
      REQUIRE(fe.isPressing() == false);

      REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == false);
      REQUIRE(fe.isPressing() == false);
    }
    {
      Flags f(ModifierFlag::SHIFT_L);
      FlagStatus currentFlags(f);
      Vector_ModifierFlag fromFlags;
      fromFlags.push_back(ModifierFlag::SHIFT_L);

      REQUIRE(fe.changePressingState(*down_return, currentFlags, fromFlags) == true);
      REQUIRE(fe.isPressing() == true);

      REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == true);
      REQUIRE(fe.isPressing() == false);
    }
    {
      Flags f(ModifierFlag::SHIFT_L);
      FlagStatus currentFlags(f);
      Vector_ModifierFlag fromFlags;
      fromFlags.push_back(ModifierFlag::SHIFT_L);

      REQUIRE(fe.changePressingState(*down_return, currentFlags, fromFlags) == true);
      REQUIRE(fe.isPressing() == true);

      // Change state even if currentFlags lacks flags when key is pressing.
      // This behavior is necessary for this case.
      // - shift down
      // - return down (shift-return is pressed.)
      // - shift up
      // - return up (shift-return is released.)
      currentFlags.reset();
      REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == true);
      REQUIRE(fe.isPressing() == false);
      // return false if call changePressingState once again.
      REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == false);
    }
  }
  {
    FromEvent fe(KeyCode::SPACE);
    FlagStatus currentFlags;
    Vector_ModifierFlag fromFlags;
    REQUIRE(fe.changePressingState(*down_return, currentFlags, fromFlags) == false);
    REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == false);
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    Flags f(ModifierFlag::SHIFT_L);
    FlagStatus currentFlags(f);
    Vector_ModifierFlag fromFlags;
    REQUIRE(fe.changePressingState(*down_shift, currentFlags, fromFlags) == true);
    REQUIRE(fe.isPressing() == true);

    currentFlags.reset();
    REQUIRE(fe.changePressingState(*up_shift, currentFlags, fromFlags) == true);
    REQUIRE(fe.isPressing() == false);
  }
  // multiple times
  {
    FromEvent fe(KeyCode::RETURN);
    FlagStatus currentFlags;
    Vector_ModifierFlag fromFlags;
    REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == false);
    REQUIRE(fe.changePressingState(*down_return, currentFlags, fromFlags) == true);
    REQUIRE(fe.changePressingState(*down_return, currentFlags, fromFlags) == true);
    REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == true);
    REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == true);
    REQUIRE(fe.changePressingState(*up_return, currentFlags, fromFlags) == false);
  }
}

TEST_CASE("isTargetDownEvent", "[Generic]") {
  {
    FromEvent fe(KeyCode::RETURN);
    REQUIRE(fe.isTargetDownEvent(*down_return) == true);
    REQUIRE(fe.isTargetUpEvent(*down_return) == false);

    REQUIRE(fe.isTargetDownEvent(*up_return) == false);
    REQUIRE(fe.isTargetUpEvent(*up_return) == true);

    REQUIRE(fe.isTargetDownEvent(*down_shift) == false);
    REQUIRE(fe.isTargetUpEvent(*down_shift) == false);
    REQUIRE(fe.isTargetDownEvent(*up_shift) == false);
    REQUIRE(fe.isTargetUpEvent(*up_shift) == false);
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    REQUIRE(fe.isTargetDownEvent(*down_shift) == true);
    REQUIRE(fe.isTargetUpEvent(*down_shift) == false);

    REQUIRE(fe.isTargetDownEvent(*up_shift) == false);
    REQUIRE(fe.isTargetUpEvent(*up_shift) == true);
  }
}
