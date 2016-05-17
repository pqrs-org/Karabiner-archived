#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>

#include "ButtonStatus.hpp"
#include "Config.hpp"
#include "KeyCode.hpp"

using namespace org_pqrs_Karabiner;
Config config;

std::ostream& operator<<(std::ostream& os, const EventType& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const KeyboardType& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const ModifierFlag& v) { return os << v.getRawBits(); }
std::ostream& operator<<(std::ostream& os, const Flags& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const KeyCode& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const ConsumerKeyCode& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const PointingButton& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const Buttons& v) { return os << v.get(); }

TEST_CASE("all", "[Buttons]") {
  REQUIRE(ButtonStatus::initialize() == true);

  REQUIRE(Buttons() == ButtonStatus::makeButtons());

  // ------------------------------------------------------------
  // set
  {
    ButtonStatus::set(PointingButton(1 << 3), true);
    REQUIRE(Buttons((1 << 3)) == ButtonStatus::makeButtons());

    ButtonStatus::set(PointingButton(1 << 11), false);
    REQUIRE(Buttons((1 << 3)) == ButtonStatus::makeButtons());

    ButtonStatus::set(PointingButton(1 << 11), true);
    REQUIRE(Buttons((1 << 3)) == ButtonStatus::makeButtons());

    ButtonStatus::set(PointingButton(1 << 11), true);
    REQUIRE(Buttons((1 << 3) | (1 << 11)) == ButtonStatus::makeButtons());

    ButtonStatus::set(PointingButton(1 << 3), false);
    REQUIRE(Buttons((1 << 11)) == ButtonStatus::makeButtons());

    ButtonStatus::set(PointingButton(1 << 1), true);
    REQUIRE(Buttons((1 << 1) | (1 << 11)) == ButtonStatus::makeButtons());
  }

  // ------------------------------------------------------------
  // increase & decrease
  ButtonStatus::increase(Buttons(1 << 9));
  REQUIRE(Buttons((1 << 1) | (1 << 9) | (1 << 11)) == ButtonStatus::makeButtons());

  ButtonStatus::decrease(Buttons((1 << 4) | (1 << 8)));
  REQUIRE(Buttons((1 << 1) | (1 << 9) | (1 << 11)) == ButtonStatus::makeButtons());

  // ------------------------------------------------------------
  // reset
  ButtonStatus::reset();
  REQUIRE(Buttons() == ButtonStatus::makeButtons());

  // ------------------------------------------------------------
  // lock
  ButtonStatus::lock_increase(Buttons((1 << 1) | (1 << 3)));
  ButtonStatus::increase(Buttons(1 << 5));
  REQUIRE(Buttons((1 << 1) | (1 << 3) | (1 << 5)) == ButtonStatus::makeButtons());
  ButtonStatus::reset();
  REQUIRE(Buttons((1 << 1) | (1 << 3)) == ButtonStatus::makeButtons());
  ButtonStatus::lock_clear();
  REQUIRE(Buttons() == ButtonStatus::makeButtons());
}
