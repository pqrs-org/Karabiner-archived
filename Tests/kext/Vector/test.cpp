#include <ostream>
#include <gtest/gtest.h>
#include "Vector.hpp"

class TestItem {
public:
  static int counter;

  TestItem(void) : value_(0) { ++counter; }
  TestItem(size_t v) : value_(v) { ++counter; }
  ~TestItem(void) {
    --counter;
    // std::cout << "~TestItem: " << value_ << std::endl;
  }

  size_t v(void) { return value_; }

private:
  size_t value_;
};
int TestItem::counter = 0;

DECLARE_VECTOR(TestItem);

TEST(Vector, push_back) {
  EXPECT_EQ(0, TestItem::counter);

  Vector_TestItem v;
  const size_t MAXITEM = 10;

  EXPECT_TRUE(v.empty());
  EXPECT_EQ(static_cast<size_t>(0), v.capacity());

  for (size_t i = 0; i < MAXITEM; ++i) {
    // push twice
    EXPECT_EQ(i * 2, v.size());
    EXPECT_EQ(i * 2, v.capacity());
    v.push_back(TestItem(i)).push_back(TestItem(i));
    EXPECT_EQ(i * 2 + 2, v.size());
    EXPECT_EQ(i * 2 + 2, v.capacity());
    EXPECT_EQ(i * 2 + 2, TestItem::counter);
  }

  for (size_t i = 0; i < MAXITEM; ++i) {
    EXPECT_EQ(i, v[i * 2 + 0].v());
    EXPECT_EQ(i, v[i * 2 + 1].v());
  }

  EXPECT_TRUE(! v.empty());
}

TEST(Vector, frontback) {
  EXPECT_EQ(0, TestItem::counter);

  Vector_TestItem v;

  const size_t MAXITEM = 10;
  for (size_t i = 0; i < MAXITEM; ++i) {
    v.push_back(TestItem(i));
  }

  EXPECT_EQ(static_cast<size_t>(0), v.front().v());
  EXPECT_EQ(static_cast<size_t>(MAXITEM - 1), v.back().v());
}

TEST(Vector, reserve) {
  EXPECT_EQ(0, TestItem::counter);

  Vector_TestItem v;
  const size_t MAXITEM = 10;

  v.reserve(1000);

  EXPECT_TRUE(v.empty());
  EXPECT_EQ(static_cast<size_t>(1000), v.capacity());

  for (size_t i = 0; i < MAXITEM; ++i) {
    // push twice
    EXPECT_EQ(i * 2, v.size());
    EXPECT_EQ(static_cast<size_t>(1000), v.capacity());
    v.push_back(TestItem(i)).push_back(TestItem(i));
    EXPECT_EQ(i * 2 + 2, v.size());
    EXPECT_EQ(static_cast<size_t>(1000), v.capacity());
  }

  for (size_t i = 0; i < MAXITEM; ++i) {
    EXPECT_EQ(i, v[i * 2 + 0].v());
    EXPECT_EQ(i, v[i * 2 + 1].v());
  }

  EXPECT_TRUE(! v.empty());
}

TEST(Vector, clear) {
  EXPECT_EQ(0, TestItem::counter);

  Vector_TestItem v;
  v.reserve(1000);
  EXPECT_EQ(static_cast<size_t>(1000), v.capacity());

  v.clear();
  EXPECT_EQ(static_cast<size_t>(0), v.capacity());

  // --------------------
  v.push_back(TestItem(1));
  v.push_back(TestItem(2));
  EXPECT_EQ(static_cast<size_t>(2), v.capacity());

  v.clear();
  EXPECT_EQ(static_cast<size_t>(0), v.capacity());
}
