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
