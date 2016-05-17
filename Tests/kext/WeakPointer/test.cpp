#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>

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

TEST_CASE("expired", "[WeakPointer]") {
  auto p1 = new TestItem();
  auto p2 = new TestItem();

  WeakPointer_TestItem wp1(p1);
  WeakPointer_TestItem wp2(p2);

  REQUIRE(wp1.expired() == false);
  REQUIRE(wp2.expired() == false);

  REQUIRE(wp1->succ() == 1);
  REQUIRE(wp2->succ() == 1);
  REQUIRE(wp1->succ() == 2);
  REQUIRE(wp2->succ() == 2);

  delete p1;

  REQUIRE(wp1.expired() == true);
  REQUIRE(wp2.expired() == false);

  delete p2;

  REQUIRE(wp1.expired() == true);
  REQUIRE(wp2.expired() == true);
}

TEST_CASE("reuse_address", "[WeakPointer]") {
  auto p1 = new TestReuseAddrItem();

  WeakPointer_TestReuseAddrItem wp1(dummyAddress);

  REQUIRE(wp1.expired() == false);

  delete p1;

  REQUIRE(wp1.expired() == true);

  auto p2 = new TestReuseAddrItem();
  WeakPointer_TestReuseAddrItem wp2(dummyAddress);

  REQUIRE(wp1.expired() == true);
  REQUIRE(wp2.expired() == false);

  delete p2;
}
