#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>

#include "ButtonStatus.hpp"
#include "Config.hpp"
#include "KeyCode.hpp"

using namespace org_pqrs_Karabiner;
Config config;

TEST_CASE("all", "[Buttons]") {
  REQUIRE(ButtonStatus::initialize() == true);

  REQUIRE(ButtonStatus::makeButtons() == Buttons());

  // ------------------------------------------------------------
  // set
  {
    ButtonStatus::set(PointingButton(1 << 3), true);
    REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 3)));

    ButtonStatus::set(PointingButton(1 << 11), false);
    REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 3)));

    ButtonStatus::set(PointingButton(1 << 11), true);
    REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 3)));

    ButtonStatus::set(PointingButton(1 << 11), true);
    REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 3) | (1 << 11)));

    ButtonStatus::set(PointingButton(1 << 3), false);
    REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 11)));

    ButtonStatus::set(PointingButton(1 << 1), true);
    REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 1) | (1 << 11)));
  }

  // ------------------------------------------------------------
  // increase & decrease
  ButtonStatus::increase(Buttons(1 << 9));
  REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 1) | (1 << 9) | (1 << 11)));

  ButtonStatus::decrease(Buttons((1 << 4) | (1 << 8)));
  REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 1) | (1 << 9) | (1 << 11)));

  // ------------------------------------------------------------
  // reset
  ButtonStatus::reset();
  REQUIRE(ButtonStatus::makeButtons() == Buttons());

  // ------------------------------------------------------------
  // lock
  ButtonStatus::lock_increase(Buttons((1 << 1) | (1 << 3)));
  ButtonStatus::increase(Buttons(1 << 5));
  REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 1) | (1 << 3) | (1 << 5)));
  ButtonStatus::reset();
  REQUIRE(ButtonStatus::makeButtons() == Buttons((1 << 1) | (1 << 3)));
  ButtonStatus::lock_clear();
  REQUIRE(ButtonStatus::makeButtons() == Buttons());
}
