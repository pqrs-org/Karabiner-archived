#include <ostream>
#include <gtest/gtest.h>
#include "keycode.hpp"
#include "FlagStatus.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

std::ostream& operator<<(std::ostream& os, const EventType& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const KeyboardType& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const ModifierFlag& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const Flags& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const KeyCode& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const ConsumerKeyCode& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const PointingButton& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const Buttons& v) { return os << v.get(); }

TEST(FlagStatus, makeFlags) {
  ASSERT_TRUE(FlagStatus::initialize());

  EXPECT_EQ(Flags(), FlagStatus::makeFlags());

  FlagStatus::set();
  EXPECT_EQ(Flags(), FlagStatus::makeFlags());

  FlagStatus::set(KeyCode::A, 0);
  EXPECT_EQ(Flags(), FlagStatus::makeFlags());

  // down SHIFT_L
  FlagStatus::set(KeyCode::SHIFT_L, ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), FlagStatus::makeFlags());

  // no effect with ModifierFlag::NONE
  FlagStatus::set(KeyCode::A, ModifierFlag::NONE);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), FlagStatus::makeFlags());

  // down CONTROL_
  FlagStatus::set(KeyCode::CONTROL_L, ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_L), FlagStatus::makeFlags());

  // down A
  FlagStatus::set(KeyCode::A, ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_L), FlagStatus::makeFlags());

  // up SHIFT_L
  FlagStatus::set(KeyCode::SHIFT_L, ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::CONTROL_L), FlagStatus::makeFlags());

  // up CONTROL_L
  FlagStatus::set(KeyCode::CONTROL_L, 0);
  EXPECT_EQ(Flags(), FlagStatus::makeFlags());
}

TEST(FlagStatus, increase) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::increase(ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), FlagStatus::makeFlags());

  FlagStatus::increase(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L), FlagStatus::makeFlags());
}

TEST(FlagStatus, decrease) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::increase(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), FlagStatus::makeFlags());

  FlagStatus::decrease(ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());
}

TEST(FlagStatus, CapsLock) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::set(KeyCode::CAPSLOCK, ModifierFlag::CAPSLOCK);
}
