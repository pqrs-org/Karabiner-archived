#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>

#include "List.hpp"

using namespace org_pqrs_Karabiner;

int allocatecount = 0;

class TestItem final : public List::Item {
public:
  TestItem(int v) : value_(v) { ++allocatecount; }
  virtual ~TestItem(void) {
    --allocatecount;
  };

  int v(void) { return value_; }

private:
  int value_;
};

TEST_CASE("push_back", "[List]") {
  List list;
  TestItem* p = nullptr;
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  list.push_back(nullptr);
  REQUIRE(list.size() == static_cast<size_t>(0));

  // ------------------------------------------------------------
  REQUIRE(list.size() == static_cast<size_t>(0));
  for (int i = 0; i < MAXITEM; ++i) {
    list.push_back(new TestItem(i));
    REQUIRE(list.size() == static_cast<size_t>(i + 1));
  }

  for (int i = 0; i < MAXITEM; ++i) {
    p = dynamic_cast<TestItem*>(list.safe_front());
    REQUIRE(p != nullptr);
    REQUIRE(p->v() == i);

    list.pop_front();
    REQUIRE(list.size() == static_cast<size_t>(MAXITEM - (i + 1)));
  }
  REQUIRE(list.size() == static_cast<size_t>(0));
  REQUIRE(allocatecount == 0);

  // ------------------------------------------------------------
  // check prev/next value.
  TestItem* item1 = new TestItem(1);
  TestItem* item2 = new TestItem(2);
  REQUIRE(item1->getprev() == nullptr);
  REQUIRE(item1->getnext() == nullptr);
  REQUIRE(item2->getprev() == nullptr);
  REQUIRE(item2->getnext() == nullptr);

  list.push_back(item1);
  REQUIRE(item1->getprev() == nullptr);
  REQUIRE(item1->getnext() == nullptr);
  list.push_back(item2);
  REQUIRE(item1->getprev() == nullptr);
  REQUIRE(item1->getnext() == item2);
  REQUIRE(item2->getprev() == item1);
  REQUIRE(item2->getnext() == nullptr);

  list.pop_front();
  REQUIRE(item2->getprev() == nullptr);
  REQUIRE(item2->getnext() == nullptr);

  list.pop_front();
  REQUIRE(allocatecount == 0);
}

TEST_CASE("push_front", "[List]") {
  List list;
  TestItem* p = nullptr;
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  list.push_front(nullptr);
  REQUIRE(list.size() == static_cast<size_t>(0));

  // ------------------------------------------------------------
  REQUIRE(list.size() == static_cast<size_t>(0));
  for (int i = 0; i < MAXITEM; ++i) {
    list.push_front(new TestItem(i));
    REQUIRE(list.size() == static_cast<size_t>(i + 1));
  }

  for (int i = 0; i < MAXITEM; ++i) {
    p = dynamic_cast<TestItem*>(list.safe_front());
    REQUIRE(p != nullptr);
    REQUIRE(p->v() == MAXITEM - (i + 1));

    list.pop_front();
    REQUIRE(list.size() == static_cast<size_t>(MAXITEM - (i + 1)));
  }
  REQUIRE(list.size() == static_cast<size_t>(0));
  REQUIRE(allocatecount == 0);

  // ------------------------------------------------------------
  // check prev/next value.
  TestItem* item1 = new TestItem(1);
  TestItem* item2 = new TestItem(2);
  REQUIRE(item1->getprev() == nullptr);
  REQUIRE(item1->getnext() == nullptr);
  REQUIRE(item2->getprev() == nullptr);
  REQUIRE(item2->getnext() == nullptr);

  list.push_front(item1);
  REQUIRE(item1->getprev() == nullptr);
  REQUIRE(item1->getnext() == nullptr);
  list.push_front(item2);
  REQUIRE(item1->getprev() == item2);
  REQUIRE(item1->getnext() == nullptr);
  REQUIRE(item2->getprev() == nullptr);
  REQUIRE(item2->getnext() == item1);

  list.pop_front();
  REQUIRE(item1->getprev() == nullptr);
  REQUIRE(item1->getnext() == nullptr);

  list.pop_front();
  REQUIRE(allocatecount == 0);
}

TEST_CASE("safe_front", "[List]") {
  List list;
  list.safe_front();
  list.safe_front();
  list.safe_front();
}
TEST_CASE("safe_back", "[List]") {
  List list;
  list.safe_back();
  list.safe_back();
  list.safe_back();
}
TEST_CASE("pop_front", "[List]") {
  List list;
  list.pop_front();
  list.pop_front();
  list.pop_front();
}
TEST_CASE("pop_back", "[List]") {
  List list;
  list.pop_back();
  list.pop_back();
  list.pop_back();

  list.push_back(new TestItem(1));
  list.push_back(new TestItem(2));
  list.pop_back();
  REQUIRE(list.size() == static_cast<size_t>(1));

  TestItem* p = static_cast<TestItem*>(list.safe_back());
  REQUIRE(p->v() == static_cast<size_t>(1));
}

TEST_CASE("insert", "[List]") {
  List list;
  TestItem* p = nullptr;

  list.insert(nullptr, nullptr);

  // push_front if first argument == nullptr.
  p = static_cast<TestItem*>(list.insert(nullptr, new TestItem(1))); // [1]
  p = static_cast<TestItem*>(list.insert(nullptr, new TestItem(2))); // [2,1]

  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 2);
  REQUIRE(static_cast<TestItem*>(list.safe_front()->getnext())->v() == 1);
  REQUIRE(static_cast<TestItem*>(list.safe_back())->v() == 1);
  REQUIRE(static_cast<TestItem*>(list.safe_back()->getprev())->v() == 2);

  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 2);
  list.pop_front();
  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 1);
  list.pop_front();

  p = static_cast<TestItem*>(list.insert(nullptr, new TestItem(1))); // [1]
  p = static_cast<TestItem*>(list.insert(p, new TestItem(2)));       // [2,1]

  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 2);
  REQUIRE(static_cast<TestItem*>(list.safe_front()->getnext())->v() == 1);
  REQUIRE(static_cast<TestItem*>(list.safe_back())->v() == 1);
  REQUIRE(static_cast<TestItem*>(list.safe_back()->getprev())->v() == 2);

  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 2);
  list.pop_front();
  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 1);
  list.pop_front();

  p = static_cast<TestItem*>(list.insert(nullptr, new TestItem(1)));          // [1]
  p = static_cast<TestItem*>(list.insert(p, new TestItem(2)));                // [2,1]
  p = static_cast<TestItem*>(list.insert(list.safe_back(), new TestItem(3))); // [2,3,1]

  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 2);
  REQUIRE(static_cast<TestItem*>(list.safe_back())->v() == 1);

  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 2);
  list.pop_front();
  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 3);
  list.pop_front();
  REQUIRE(static_cast<TestItem*>(list.safe_front())->v() == 1);
  list.pop_front();
}

TEST_CASE("erase_and_delete", "[List]") {
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  for (int erase_index = 0; erase_index < MAXITEM; ++erase_index) {
    List list;

    REQUIRE(list.size() == static_cast<size_t>(0));
    for (int i = 0; i < MAXITEM; ++i) {
      list.push_back(new TestItem(i));
      REQUIRE(list.size() == static_cast<size_t>(i + 1));
    }

    TestItem* p = static_cast<TestItem*>(list.safe_front());
    for (int i = 0; i < erase_index; ++i) {
      p = static_cast<TestItem*>(p->getnext());
    }
    TestItem* next = static_cast<TestItem*>(p->getnext());
    REQUIRE(list.erase_and_delete(p) == next);

    // check
    p = static_cast<TestItem*>(list.safe_front());
    for (int i = 0; i < MAXITEM; ++i) {
      if (i == erase_index) continue;
      REQUIRE(p->v() == i);
      p = static_cast<TestItem*>(p->getnext());
    }
    REQUIRE(list.size() == static_cast<size_t>(MAXITEM - 1));
  }
}

TEST_CASE("clear", "[List]") {
  List list;
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  REQUIRE(list.size() == static_cast<size_t>(0));
  for (int i = 0; i < MAXITEM; ++i) {
    list.push_back(new TestItem(i));
    REQUIRE(list.size() == static_cast<size_t>(i + 1));
  }
  list.clear();
  REQUIRE(list.size() == static_cast<size_t>(0));
  REQUIRE(allocatecount == 0);
}
