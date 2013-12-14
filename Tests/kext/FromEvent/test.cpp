#include <ostream>
#include <gtest/gtest.h>
#include "FromEvent.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

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

TEST(Generic, isTargetDownEvent) {
  Params_KeyboardEventCallBack::auto_ptr down_return(
    Params_KeyboardEventCallBack::alloc(
      EventType::DOWN,
      Flags(0),
      KeyCode::RETURN,
      CharCode(0),
      CharSet(0),
      OrigCharCode(0),
      OrigCharSet(0),
      KeyboardType(0),
      false));

  Params_KeyboardEventCallBack::auto_ptr up_return(
    Params_KeyboardEventCallBack::alloc(
      EventType::UP,
      Flags(0),
      KeyCode::RETURN,
      CharCode(0),
      CharSet(0),
      OrigCharCode(0),
      OrigCharSet(0),
      KeyboardType(0),
      false));

  Params_KeyboardEventCallBack::auto_ptr down_shift(
    Params_KeyboardEventCallBack::alloc(
      EventType::MODIFY,
      Flags(ModifierFlag::SHIFT_L),
      KeyCode::SHIFT_L,
      CharCode(0),
      CharSet(0),
      OrigCharCode(0),
      OrigCharSet(0),
      KeyboardType(0),
      false));

  Params_KeyboardEventCallBack::auto_ptr up_shift(
    Params_KeyboardEventCallBack::alloc(
      EventType::MODIFY,
      Flags(0),
      KeyCode::SHIFT_L,
      CharCode(0),
      CharSet(0),
      OrigCharCode(0),
      OrigCharSet(0),
      KeyboardType(0),
      false));

  {
    FromEvent fe(KeyCode::RETURN);
    EXPECT_EQ(true,  fe.isTargetDownEvent(*down_return));
    EXPECT_EQ(false, fe.isTargetUpEvent(*down_return));
    EXPECT_EQ(false, fe.isTargetDownEvent(*up_return));
    EXPECT_EQ(true,  fe.isTargetUpEvent(*up_return));
  }
  {
    FromEvent fe(KeyCode::SPACE);
    EXPECT_EQ(false, fe.isTargetDownEvent(*down_return));
    EXPECT_EQ(false, fe.isTargetUpEvent(*down_return));
    EXPECT_EQ(false, fe.isTargetDownEvent(*up_return));
    EXPECT_EQ(false, fe.isTargetUpEvent(*up_return));
  }
  {
    FromEvent fe(KeyCode::SHIFT_L);
    EXPECT_EQ(true,  fe.isTargetDownEvent(*down_shift));
    EXPECT_EQ(false, fe.isTargetUpEvent(*down_shift));
    EXPECT_EQ(false,  fe.isTargetDownEvent(*up_shift));
    EXPECT_EQ(true, fe.isTargetUpEvent(*up_shift));
  }
}
