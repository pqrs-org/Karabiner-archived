#include <gtest/gtest.h>
#include "keycode.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

TEST(ModifierFlag, stripFN) {
  unsigned int flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_EQ(ModifierFlag::stripFN(flags), flags);
  EXPECT_EQ(ModifierFlag::stripFN(flags | ModifierFlag::FN), flags);
}

TEST(ModifierFlag, stripCURSOR) {
  unsigned int flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_EQ(ModifierFlag::stripCURSOR(flags), flags);
  EXPECT_EQ(ModifierFlag::stripCURSOR(flags | ModifierFlag::CURSOR), flags);
}

TEST(ModifierFlag, stripNONE) {
  unsigned int flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_EQ(ModifierFlag::stripNONE(flags), flags);
  EXPECT_EQ(ModifierFlag::stripNONE(flags | ModifierFlag::NONE), flags);
}
