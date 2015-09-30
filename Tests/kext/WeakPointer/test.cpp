#include <ostream>
#include <gtest/gtest.h>
#include "WeakPointer.hpp"

using namespace org_pqrs_Karabiner;

DECLARE_WEAKPOINTER(TestItem);

class TestItem final {
public:
  TestItem(void) : counter_(0) {
    WeakPointerManager_TestItem::add(this);
  }
  ~TestItem(void) {
    WeakPointerManager_TestItem::remove(this);
  }

  int succ(void) { return ++counter_; }

private:
  int counter_;
};

DEFINE_WEAKPOINTER(TestItem);

DECLARE_WEAKPOINTER(TestReuseAddrItem);

int dummy = 10;
TestReuseAddrItem* dummyAddress = reinterpret_cast<TestReuseAddrItem*>(&dummy);

class TestReuseAddrItem final {
public:
  TestReuseAddrItem(void) {
    WeakPointerManager_TestReuseAddrItem::add(dummyAddress);
  }
  ~TestReuseAddrItem(void) {
    WeakPointerManager_TestReuseAddrItem::remove(dummyAddress);
  }
};

DEFINE_WEAKPOINTER(TestReuseAddrItem);

TEST(WeakPointer, expired) {
  auto p1 = new TestItem();
  auto p2 = new TestItem();

  WeakPointer_TestItem wp1(p1);
  WeakPointer_TestItem wp2(p2);

  EXPECT_FALSE(wp1.expired());
  EXPECT_FALSE(wp2.expired());

  EXPECT_EQ(1, wp1->succ());
  EXPECT_EQ(1, wp2->succ());
  EXPECT_EQ(2, wp1->succ());
  EXPECT_EQ(2, wp2->succ());

  delete p1;

  EXPECT_TRUE(wp1.expired());
  EXPECT_FALSE(wp2.expired());

  delete p2;

  EXPECT_TRUE(wp1.expired());
  EXPECT_TRUE(wp2.expired());
}

TEST(WeakPointer, reuse_address) {
  auto p1 = new TestReuseAddrItem();

  WeakPointer_TestReuseAddrItem wp1(dummyAddress);

  EXPECT_FALSE(wp1.expired());

  delete p1;

  EXPECT_TRUE(wp1.expired());

  auto p2 = new TestReuseAddrItem();
  WeakPointer_TestReuseAddrItem wp2(dummyAddress);

  EXPECT_TRUE(wp1.expired());
  EXPECT_FALSE(wp2.expired());

  delete p2;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
