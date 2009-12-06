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

  FlagStatus::set(KeyCode::SHIFT_L, ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), FlagStatus::makeFlags());
}
