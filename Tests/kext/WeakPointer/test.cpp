#include <ostream>
#include <gtest/gtest.h>
#include "WeakPointer.hpp"

using namespace org_pqrs_Karabiner;

DECLARE_WEAKPOINTER(TestItem);

class TestItem final {
public:
  TestItem(void) {
    WeakPointer_TestItem::add(this);
  };
  ~TestItem(void) {
    WeakPointer_TestItem::remove(this);
  }
};

DEFINE_WEAKPOINTER(TestItem);

TEST(WeakPointer, expired) {
  auto p1 = new TestItem();
  auto p2 = new TestItem();

  EXPECT_FALSE(WeakPointer_TestItem::expired(p1));
  EXPECT_FALSE(WeakPointer_TestItem::expired(p2));

  delete p1;

  EXPECT_TRUE(WeakPointer_TestItem::expired(p1));
  EXPECT_FALSE(WeakPointer_TestItem::expired(p2));

  delete p2;

  EXPECT_TRUE(WeakPointer_TestItem::expired(p1));
  EXPECT_TRUE(WeakPointer_TestItem::expired(p2));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
