#include <ostream>
#include <gtest/gtest.h>
#include "KeyCode.hpp"
#include "ButtonStatus.hpp"
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

TEST(ButtonStatus, all) {
  ASSERT_TRUE(ButtonStatus::initialize());

  EXPECT_EQ(Buttons(), ButtonStatus::makeButtons());

  Buttons previous(0);
  int diff = 0;

  // ------------------------------------------------------------
  // set
  Buttons current((1 << 4) | (1 << 11));
  diff = ButtonStatus::set(current, previous);
  previous = current;
  EXPECT_EQ(Buttons(current), ButtonStatus::makeButtons());
  EXPECT_EQ(Buttons(current), ButtonStatus::justPressed());
  EXPECT_EQ(Buttons(0), ButtonStatus::justReleased());
  EXPECT_EQ(2, diff);

  current = ((1 << 4));
  diff = ButtonStatus::set(current, previous);
  previous = current;
  EXPECT_EQ(Buttons(current), ButtonStatus::makeButtons());
  EXPECT_EQ(Buttons(0), ButtonStatus::justPressed());
  EXPECT_EQ(Buttons((1 << 11)), ButtonStatus::justReleased());
  EXPECT_EQ(-1, diff);

  current = ((1 << 4) | (1 << 8));
  diff = ButtonStatus::set(current, previous);
  previous = current;
  EXPECT_EQ(Buttons(current), ButtonStatus::makeButtons());
  EXPECT_EQ(Buttons((1 <<  8)), ButtonStatus::justPressed());
  EXPECT_EQ(Buttons(0), ButtonStatus::justReleased());
  EXPECT_EQ(1, diff);

  // ------------------------------------------------------------
  // increase & decrease
  ButtonStatus::increase(1 << 9);

  ButtonStatus::set(current, previous);
  previous = current;
  EXPECT_EQ(Buttons(current | (1 << 9)), ButtonStatus::makeButtons());

  ButtonStatus::decrease((1 << 4) | (1 << 8));
  EXPECT_EQ(Buttons(1 << 9), ButtonStatus::makeButtons());

  // ------------------------------------------------------------
  // reset
  ButtonStatus::reset();
  EXPECT_EQ(Buttons(), ButtonStatus::makeButtons());
}
