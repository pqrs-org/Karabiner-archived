#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>
#include <stdexcept>

#include "DeltaBuffer.hpp"

using namespace org_pqrs_Karabiner;

TEST_CASE("push", "[DeltaBuffer]") {
  DeltaBuffer deltaBuffer;

  REQUIRE(deltaBuffer.sum() == 0);

  deltaBuffer.push(-1);
  REQUIRE(deltaBuffer.sum() == -1);

  // fill buffer
  REQUIRE(deltaBuffer.isFull() == false);
  int sum = 0;
  for (;;) {
    deltaBuffer.push(-1);
    if (sum == deltaBuffer.sum()) break;
    sum = deltaBuffer.sum();
  }
  REQUIRE(deltaBuffer.isFull() == true);

  deltaBuffer.push(-3); // replace -1 with -3.
  sum -= 2;
  REQUIRE(deltaBuffer.sum() == sum);

  deltaBuffer.push(0); // replace -1  with 0.
  sum += 1;
  REQUIRE(deltaBuffer.sum() == sum);

  // reverse direction
  REQUIRE(sum != 0);
  deltaBuffer.push(1);
  sum = 1;
  REQUIRE(deltaBuffer.sum() == sum);
  REQUIRE(deltaBuffer.isFull() == false);

  // clear
  while (!deltaBuffer.isFull()) {
    deltaBuffer.push(1);
  }
  REQUIRE(deltaBuffer.sum() != 0);
  REQUIRE(deltaBuffer.isFull() != 0);

  deltaBuffer.clear();

  REQUIRE(deltaBuffer.sum() == 0);
  REQUIRE(deltaBuffer.isFull() == false);
}
