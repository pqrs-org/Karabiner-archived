#include <gtest/gtest.h>
#include "keycode.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

TEST(ModifierFlag, stripFN) {
  // no FN
  EXPECT_EQ(ModifierFlag::stripFN(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R),
                                  ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R);

  EXPECT_EQ(ModifierFlag::stripFN(ModifierFlag::SHIFT_L | ModifierFlag::FN),
                                  ModifierFlag::SHIFT_L);
  EXPECT_EQ(ModifierFlag::stripFN(ModifierFlag::CURSOR | ModifierFlag::FN),
                                  ModifierFlag::CURSOR);
}

TEST(ModifierFlag, stripCURSOR) {
  // no CURSOR
  EXPECT_EQ(ModifierFlag::stripCURSOR(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R),
                                      ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R);

  EXPECT_EQ(ModifierFlag::stripCURSOR(ModifierFlag::SHIFT_L | ModifierFlag::CURSOR),
                                      ModifierFlag::SHIFT_L);
  EXPECT_EQ(ModifierFlag::stripCURSOR(ModifierFlag::FN | ModifierFlag::CURSOR),
                                      ModifierFlag::FN);
}

TEST(ModifierFlag, stripNONE) {
  // no NONE
  EXPECT_EQ(ModifierFlag::stripNONE(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R),
                                    ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R);

  EXPECT_EQ(ModifierFlag::stripNONE(ModifierFlag::SHIFT_L | ModifierFlag::NONE),
                                    ModifierFlag::SHIFT_L);
  EXPECT_EQ(ModifierFlag::stripNONE(ModifierFlag::CURSOR | ModifierFlag::NONE),
                                    ModifierFlag::CURSOR);
}
