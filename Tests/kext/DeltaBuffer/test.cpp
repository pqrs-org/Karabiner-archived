#include <ostream>
#include <stdexcept>
#include <gtest/gtest.h>
#include "DeltaBuffer.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

TEST(DeltaBuffer, push) {
  DeltaBuffer deltaBuffer;

  EXPECT_EQ(0, deltaBuffer.sum());

  deltaBuffer.push(-1);
  EXPECT_EQ(-1, deltaBuffer.sum());

  // fill buffer
  EXPECT_TRUE(! deltaBuffer.isFull());
  int sum = 0;
  for (;;) {
    deltaBuffer.push(-1);
    if (sum == deltaBuffer.sum()) break;
    sum = deltaBuffer.sum();
  }
  EXPECT_TRUE(deltaBuffer.isFull());

  deltaBuffer.push(-3); // replace -1 with -3.
  sum -= 2;
  EXPECT_EQ(sum, deltaBuffer.sum());

  deltaBuffer.push(0); // replace -1  with 0.
  sum += 1;
  EXPECT_EQ(sum, deltaBuffer.sum());

  // reverse direction
  EXPECT_NE(sum, 0);
  deltaBuffer.push(1);
  sum = 1;
  EXPECT_EQ(sum, deltaBuffer.sum());
  EXPECT_TRUE(! deltaBuffer.isFull());

  // clear
  while (! deltaBuffer.isFull()) {
    deltaBuffer.push(1);
  }
  EXPECT_NE(0, deltaBuffer.sum());
  EXPECT_NE(0, deltaBuffer.isFull());

  deltaBuffer.clear();

  EXPECT_EQ(0, deltaBuffer.sum());
  EXPECT_TRUE(! deltaBuffer.isFull());
}
