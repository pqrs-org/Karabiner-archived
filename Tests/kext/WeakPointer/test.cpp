#include <ostream>
#include <gtest/gtest.h>
#include "WeakPointer.hpp"

using namespace org_pqrs_Karabiner;

DECLARE_WEAKPOINTER(TestItem);

class TestItem final {
public:
  TestItem(void) {
    WeakPointerManager_TestItem::add(this);
  }
  ~TestItem(void) {
    WeakPointerManager_TestItem::remove(this);
  }
};

DEFINE_WEAKPOINTER(TestItem);

TEST(WeakPointer, expired) {
  auto p1 = new TestItem();
  auto p2 = new TestItem();

  WeakPointer_TestItem wp1(p1);
  WeakPointer_TestItem wp2(p2);

  EXPECT_FALSE(wp1.expired());
  EXPECT_FALSE(wp2.expired());

  delete p1;

  EXPECT_TRUE(wp1.expired());
  EXPECT_FALSE(wp2.expired());

  delete p2;

  EXPECT_TRUE(wp1.expired());
  EXPECT_TRUE(wp2.expired());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
