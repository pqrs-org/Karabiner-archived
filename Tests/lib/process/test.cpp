#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <boost/lexical_cast.hpp>
#include <ostream>

#include "pqrs/process.hpp"

TEST_CASE("execute", "[pqrs_process]") {
  std::string expected = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  std::string actual;
  int exit_code = pqrs::process::launch(std::string("/bin/echo ") + expected, actual);
  expected += "\n";
  REQUIRE(actual == expected);
  REQUIRE(exit_code == 0);

  REQUIRE(pqrs::process::launch("/bin/echo hello | /usr/bin/grep world", actual) == 1);
}
