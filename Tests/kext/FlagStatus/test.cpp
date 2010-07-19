#include <ostream>
#include <gtest/gtest.h>
#include "KeyCode.hpp"
#include "FlagStatus.hpp"
#include "Config.hpp"

using namespace org_pqrs_KeyRemap4MacBook;
Config config;

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

  FlagStatus::set(KeyCode::CAPSLOCK, 0);
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());

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

TEST(FlagStatus, getFlag) {
  ASSERT_TRUE(FlagStatus::initialize());

  EXPECT_EQ(ModifierFlag::CAPSLOCK, FlagStatus::getFlag(0));
}

TEST(FlagStatus, getLockedFlags) {
  ASSERT_TRUE(FlagStatus::initialize());

  EXPECT_EQ(Flags(0), FlagStatus::getLockedFlags());

  FlagStatus::increase(ModifierFlag::SHIFT_L);
  FlagStatus::temporary_increase(ModifierFlag::SHIFT_R);
  FlagStatus::lock_increase(ModifierFlag::COMMAND_L);
  FlagStatus::lock_increase(ModifierFlag::OPTION_L);
  EXPECT_EQ(ModifierFlag::COMMAND_L | ModifierFlag::OPTION_L, FlagStatus::getLockedFlags());
}

TEST(FlagStatus, increase) {
  ASSERT_TRUE(FlagStatus::initialize());

  // Do nothing with ModifierFlag::NONE.
  FlagStatus::increase(ModifierFlag::NONE);
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());

  FlagStatus::increase(ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), FlagStatus::makeFlags());

  FlagStatus::increase(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L), FlagStatus::makeFlags());

  FlagStatus::increase(ModifierFlag::NONE);
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

  // Do nothing with ModifierFlag::NONE.
  FlagStatus::temporary_increase(ModifierFlag::NONE);
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());

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

  // Do nothing with ModifierFlag::NONE.
  FlagStatus::lock_increase(ModifierFlag::NONE);
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());

  FlagStatus::lock_increase(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());

  // lock don't cancel by reset & set.
  FlagStatus::reset();
  FlagStatus::set(KeyCode::A, 0);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());

  FlagStatus::lock_decrease(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(), FlagStatus::makeFlags());
}

TEST(FlagStatus, lock_toggle) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::lock_increase(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());

  FlagStatus::lock_toggle(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());

  FlagStatus::lock_toggle(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());
}

TEST(FlagStatus, lock_clear) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::lock_increase(ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L);
  EXPECT_EQ(ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L, FlagStatus::makeFlags());

  FlagStatus::lock_clear();
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());
}

TEST(FlagStatus, sticky_increase) {
  ASSERT_TRUE(FlagStatus::initialize());

  // Do nothing with ModifierFlag::NONE.
  FlagStatus::sticky_increase(ModifierFlag::NONE);
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());

  FlagStatus::sticky_increase(ModifierFlag::COMMAND_L | ModifierFlag::FN);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::FN), FlagStatus::makeFlags());

  FlagStatus::sticky_decrease(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::FN), FlagStatus::makeFlags());
}

TEST(FlagStatus, sticky_toggle) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::sticky_increase(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());

  FlagStatus::sticky_toggle(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());

  FlagStatus::sticky_toggle(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), FlagStatus::makeFlags());
}

TEST(FlagStatus, sticky_clear) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::sticky_increase(ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L);
  EXPECT_EQ(ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L, FlagStatus::makeFlags());

  FlagStatus::sticky_clear();
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());
}

TEST(FlagStatus, CapsLock) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::set(KeyCode::CAPSLOCK, ModifierFlag::CAPSLOCK);
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), FlagStatus::makeFlags());

  FlagStatus::reset();

  FlagStatus::set(KeyCode::A, ModifierFlag::CAPSLOCK);
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), FlagStatus::makeFlags());

  // from other keyboard
  FlagStatus::set(KeyCode::A, 0);
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), FlagStatus::makeFlags());

  FlagStatus::set(KeyCode::A, ModifierFlag::CAPSLOCK);
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), FlagStatus::makeFlags());

  // reset
  FlagStatus::set(KeyCode::CAPSLOCK, 0);
  EXPECT_EQ(Flags(), FlagStatus::makeFlags());

  // soft caps
  FlagStatus::lock_increase(ModifierFlag::CAPSLOCK);
  FlagStatus::set(KeyCode::A, 0);
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), FlagStatus::makeFlags());

  // soft caps will be canceled by hardware caps
  FlagStatus::set(KeyCode::CAPSLOCK, 0);
  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());
}

TEST(FlagStatus, ScopedTemporaryFlagsChanger) {
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::increase(ModifierFlag::SHIFT_L);
  FlagStatus::increase(ModifierFlag::SHIFT_L);
  FlagStatus::increase(ModifierFlag::SHIFT_L);
  FlagStatus::increase(ModifierFlag::SHIFT_L);
  FlagStatus::increase(ModifierFlag::SHIFT_R);
  FlagStatus::temporary_increase(ModifierFlag::CONTROL_L);
  FlagStatus::lock_increase(ModifierFlag::COMMAND_R);
  FlagStatus::sticky_increase(ModifierFlag::OPTION_R);

  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_L | ModifierFlag::COMMAND_R | ModifierFlag::OPTION_R), FlagStatus::makeFlags());

  {
    FlagStatus::ScopedTemporaryFlagsChanger stfc(ModifierFlag::FN | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_R);

    EXPECT_EQ(Flags(ModifierFlag::FN | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_R), FlagStatus::makeFlags());
  }

  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_L | ModifierFlag::COMMAND_R | ModifierFlag::OPTION_R), FlagStatus::makeFlags());

  FlagStatus::decrease(ModifierFlag::SHIFT_L);
  FlagStatus::decrease(ModifierFlag::SHIFT_L);
  FlagStatus::decrease(ModifierFlag::SHIFT_L);

  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_L | ModifierFlag::COMMAND_R | ModifierFlag::OPTION_R), FlagStatus::makeFlags());

  FlagStatus::decrease(ModifierFlag::SHIFT_L);

  EXPECT_EQ(Flags(ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_L | ModifierFlag::COMMAND_R | ModifierFlag::OPTION_R), FlagStatus::makeFlags());

  // ------------------------------------------------------------
  ASSERT_TRUE(FlagStatus::initialize());

  FlagStatus::decrease(ModifierFlag::SHIFT_L);
  FlagStatus::decrease(ModifierFlag::SHIFT_L);

  {
    FlagStatus::ScopedTemporaryFlagsChanger stfc(ModifierFlag::SHIFT_R);
    EXPECT_EQ(Flags(ModifierFlag::SHIFT_R), FlagStatus::makeFlags());
  }

  FlagStatus::increase(ModifierFlag::SHIFT_L);
  FlagStatus::increase(ModifierFlag::SHIFT_L);

  EXPECT_EQ(Flags(0), FlagStatus::makeFlags());
}
