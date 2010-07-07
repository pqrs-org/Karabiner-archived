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

  // ------------------------------------------------------------
  // set
  {
    Buttons justPressed(1 << 3);
    Buttons justReleased(1 << 11);
    ButtonStatus::set(justPressed, justReleased);
    EXPECT_EQ(Buttons((1 << 3)), ButtonStatus::makeButtons());

    justPressed = Buttons(1 << 11);
    justReleased = Buttons(0);
    ButtonStatus::set(justPressed, justReleased);
    EXPECT_EQ(Buttons((1 << 3)), ButtonStatus::makeButtons());

    justPressed = Buttons(1 << 11);
    justReleased = Buttons(0);
    ButtonStatus::set(justPressed, justReleased);
    EXPECT_EQ(Buttons((1 << 3) | (1 << 11)), ButtonStatus::makeButtons());

    justPressed = Buttons(0);
    justReleased = Buttons(1 << 3);
    ButtonStatus::set(justPressed, justReleased);
    EXPECT_EQ(Buttons((1 << 11)), ButtonStatus::makeButtons());

    justPressed = Buttons(1 << 1);
    justReleased = Buttons(0);
    ButtonStatus::set(justPressed, justReleased);
    EXPECT_EQ(Buttons((1 << 1) | (1 << 11)), ButtonStatus::makeButtons());
  }

  // ------------------------------------------------------------
  // increase & decrease
  ButtonStatus::increase(1 << 9);
  EXPECT_EQ(Buttons((1 << 1) | (1 << 9) | (1 << 11)), ButtonStatus::makeButtons());

  ButtonStatus::decrease((1 << 4) | (1 << 8));
  EXPECT_EQ(Buttons((1 << 1) | (1 << 9) | (1 << 11)), ButtonStatus::makeButtons());

  // ------------------------------------------------------------
  // reset
  ButtonStatus::reset();
  EXPECT_EQ(Buttons(), ButtonStatus::makeButtons());
}
