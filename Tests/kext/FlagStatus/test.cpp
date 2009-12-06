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

  // All flags
  FlagStatus::reset();
  FlagStatus::set(KeyCode::CAPSLOCK, ModifierFlag::CAPSLOCK);
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::SHIFT_L, ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::SHIFT_R, ModifierFlag::SHIFT_R);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_R), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::CONTROL_L, ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::CONTROL_L), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::CONTROL_R, ModifierFlag::CONTROL_R);
  EXPECT_EQ(Flags(ModifierFlag::CONTROL_R), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::OPTION_L, ModifierFlag::OPTION_L);
  EXPECT_EQ(Flags(ModifierFlag::OPTION_L), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::OPTION_R, ModifierFlag::OPTION_R);
  EXPECT_EQ(Flags(ModifierFlag::OPTION_R), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::COMMAND_L, ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::COMMAND_R, ModifierFlag::COMMAND_R);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_R), FlagStatus::makeFlags());

  FlagStatus::reset();
  FlagStatus::set(KeyCode::FN, ModifierFlag::FN);
  EXPECT_EQ(Flags(ModifierFlag::FN), FlagStatus::makeFlags());
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

TEST(FlagStatus, temporary_increase) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::increase(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), FlagStatus::makeFlags());

  FlagStatus::temporary_increase(ModifierFlag::OPTION_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L), FlagStatus::makeFlags());

  // temporary_increase will reset by FlagStatus::set
  FlagStatus::set(KeyCode::COMMAND_L, ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), FlagStatus::makeFlags());
}

TEST(FlagStatus, temporary_decrease) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::increase(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), FlagStatus::makeFlags());

  FlagStatus::temporary_decrease(ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());

  // temporary_increase will reset by FlagStatus::set
  FlagStatus::set(KeyCode::COMMAND_L, ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), FlagStatus::makeFlags());
}

TEST(FlagStatus, lock_increase) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::lock_increase(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());

  // lock don't cancel by reset & set.
  FlagStatus::reset();
  FlagStatus::set(KeyCode::A, 0);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());

  FlagStatus::lock_decrease(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(), FlagStatus::makeFlags());
}

TEST(FlagStatus, CapsLock) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::set(KeyCode::CAPSLOCK, ModifierFlag::CAPSLOCK);
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), FlagStatus::makeFlags());

  FlagStatus::set(KeyCode::A, ModifierFlag::CAPSLOCK);
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), FlagStatus::makeFlags());
}
