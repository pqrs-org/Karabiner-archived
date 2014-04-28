#include <ostream>
#include <gtest/gtest.h>
#include "List.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

int allocatecount = 0;

class TestItem : public List::Item {
public:
  TestItem(int v) : value_(v) { ++allocatecount; }
  virtual ~TestItem(void) {
    --allocatecount;
  };

  int v(void) { return value_; }

private:
  int value_;
};

TEST(List, push_back) {
  List list;
  TestItem* p = NULL;
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  list.push_back(NULL);
  EXPECT_EQ(static_cast<size_t>(0), list.size());

  // ------------------------------------------------------------
  EXPECT_EQ(static_cast<size_t>(0), list.size());
  for (int i = 0; i < MAXITEM; ++i) {
    list.push_back(new TestItem(i));
    EXPECT_EQ(static_cast<size_t>(i + 1), list.size());
  }

  for (int i = 0; i < MAXITEM; ++i) {
    p = dynamic_cast<TestItem*>(list.front());
    EXPECT_TRUE(p != NULL);
    EXPECT_EQ(i, p->v());

    list.pop_front();
    EXPECT_EQ(static_cast<size_t>(MAXITEM - (i + 1)), list.size());
  }
  EXPECT_EQ(static_cast<size_t>(0), list.size());
  EXPECT_EQ(0, allocatecount);

  // ------------------------------------------------------------
  // check prev/next value.
  TestItem* item1 = new TestItem(1);
  TestItem* item2 = new TestItem(2);
  EXPECT_TRUE(item1->getprev() == NULL);
  EXPECT_TRUE(item1->getnext() == NULL);
  EXPECT_TRUE(item2->getprev() == NULL);
  EXPECT_TRUE(item2->getnext() == NULL);

  list.push_back(item1);
  EXPECT_TRUE(item1->getprev() == NULL);
  EXPECT_TRUE(item1->getnext() == NULL);
  list.push_back(item2);
  EXPECT_TRUE(item1->getprev() == NULL);
  EXPECT_TRUE(item1->getnext() == item2);
  EXPECT_TRUE(item2->getprev() == item1);
  EXPECT_TRUE(item2->getnext() == NULL);

  list.pop_front();
  EXPECT_TRUE(item2->getprev() == NULL);
  EXPECT_TRUE(item2->getnext() == NULL);

  list.pop_front();
  EXPECT_EQ(0, allocatecount);
}

TEST(List, push_front) {
  List list;
  TestItem* p = NULL;
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  list.push_front(NULL);
  EXPECT_EQ(static_cast<size_t>(0), list.size());

  // ------------------------------------------------------------
  EXPECT_EQ(static_cast<size_t>(0), list.size());
  for (int i = 0; i < MAXITEM; ++i) {
    list.push_front(new TestItem(i));
    EXPECT_EQ(static_cast<size_t>(i + 1), list.size());
  }

  for (int i = 0; i < MAXITEM; ++i) {
    p = dynamic_cast<TestItem*>(list.front());
    EXPECT_TRUE(p != NULL);
    EXPECT_EQ(MAXITEM - (i + 1), p->v());

    list.pop_front();
    EXPECT_EQ(static_cast<size_t>(MAXITEM - (i + 1)), list.size());
  }
  EXPECT_EQ(static_cast<size_t>(0), list.size());
  EXPECT_EQ(0, allocatecount);

  // ------------------------------------------------------------
  // check prev/next value.
  TestItem* item1 = new TestItem(1);
  TestItem* item2 = new TestItem(2);
  EXPECT_TRUE(item1->getprev() == NULL);
  EXPECT_TRUE(item1->getnext() == NULL);
  EXPECT_TRUE(item2->getprev() == NULL);
  EXPECT_TRUE(item2->getnext() == NULL);

  list.push_front(item1);
  EXPECT_TRUE(item1->getprev() == NULL);
  EXPECT_TRUE(item1->getnext() == NULL);
  list.push_front(item2);
  EXPECT_TRUE(item1->getprev() == item2);
  EXPECT_TRUE(item1->getnext() == NULL);
  EXPECT_TRUE(item2->getprev() == NULL);
  EXPECT_TRUE(item2->getnext() == item1);

  list.pop_front();
  EXPECT_TRUE(item1->getprev() == NULL);
  EXPECT_TRUE(item1->getnext() == NULL);

  list.pop_front();
  EXPECT_EQ(0, allocatecount);
}

TEST(List, pop_front) {
  List list;
  list.pop_front();
}
TEST(List, pop_back) {
  List list;
  list.pop_back();

  list.push_back(new TestItem(1));
  list.push_back(new TestItem(2));
  list.pop_back();
  EXPECT_EQ(static_cast<size_t>(1), list.size());

  TestItem* p = static_cast<TestItem*>(list.back());
  EXPECT_EQ(static_cast<size_t>(1), p->v());
}

TEST(List, insert) {
  List list;
  TestItem* p = NULL;

  list.insert(NULL, NULL);

  // push_front if first argument == NULL.
  p = static_cast<TestItem*>(list.insert(NULL, new TestItem(1))); // [1]
  p = static_cast<TestItem*>(list.insert(NULL, new TestItem(2))); // [2,1]

  EXPECT_EQ(2, static_cast<TestItem*>(list.front())->v());
  EXPECT_EQ(1, static_cast<TestItem*>(list.front()->getnext())->v());
  EXPECT_EQ(1, static_cast<TestItem*>(list.back())->v());
  EXPECT_EQ(2, static_cast<TestItem*>(list.back()->getprev())->v());

  EXPECT_EQ(2, static_cast<TestItem*>(list.front())->v());
  list.pop_front();
  EXPECT_EQ(1, static_cast<TestItem*>(list.front())->v());
  list.pop_front();

  p = static_cast<TestItem*>(list.insert(NULL, new TestItem(1))); // [1]
  p = static_cast<TestItem*>(list.insert(p, new TestItem(2))); // [2,1]

  EXPECT_EQ(2, static_cast<TestItem*>(list.front())->v());
  EXPECT_EQ(1, static_cast<TestItem*>(list.front()->getnext())->v());
  EXPECT_EQ(1, static_cast<TestItem*>(list.back())->v());
  EXPECT_EQ(2, static_cast<TestItem*>(list.back()->getprev())->v());

  EXPECT_EQ(2, static_cast<TestItem*>(list.front())->v());
  list.pop_front();
  EXPECT_EQ(1, static_cast<TestItem*>(list.front())->v());
  list.pop_front();

  p = static_cast<TestItem*>(list.insert(NULL, new TestItem(1))); // [1]
  p = static_cast<TestItem*>(list.insert(p, new TestItem(2))); // [2,1]
  p = static_cast<TestItem*>(list.insert(list.back(), new TestItem(3))); // [2,3,1]

  EXPECT_EQ(2, static_cast<TestItem*>(list.front())->v());
  EXPECT_EQ(1, static_cast<TestItem*>(list.back())->v());

  EXPECT_EQ(2, static_cast<TestItem*>(list.front())->v());
  list.pop_front();
  EXPECT_EQ(3, static_cast<TestItem*>(list.front())->v());
  list.pop_front();
  EXPECT_EQ(1, static_cast<TestItem*>(list.front())->v());
  list.pop_front();
}

TEST(List, erase_and_delete) {
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  for (int erase_index = 0; erase_index < MAXITEM; ++erase_index) {
    List list;

    EXPECT_EQ(static_cast<size_t>(0), list.size());
    for (int i = 0; i < MAXITEM; ++i) {
      list.push_back(new TestItem(i));
      EXPECT_EQ(static_cast<size_t>(i + 1), list.size());
    }

    TestItem* p = static_cast<TestItem*>(list.front());
    for (int i = 0; i < erase_index; ++i) {
      p = static_cast<TestItem*>(p->getnext());
    }
    TestItem* next = static_cast<TestItem*>(p->getnext());
    EXPECT_EQ(next, list.erase_and_delete(p));

    // check
    p = static_cast<TestItem*>(list.front());
    for (int i = 0; i < MAXITEM; ++i) {
      if (i == erase_index) continue;
      EXPECT_EQ(i, p->v());
      p = static_cast<TestItem*>(p->getnext());
    }
    EXPECT_EQ(static_cast<size_t>(MAXITEM - 1), list.size());
  }
}

TEST(List, clear) {
  List list;
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  EXPECT_EQ(static_cast<size_t>(0), list.size());
  for (int i = 0; i < MAXITEM; ++i) {
    list.push_back(new TestItem(i));
    EXPECT_EQ(static_cast<size_t>(i + 1), list.size());
  }
  list.clear();
  EXPECT_EQ(static_cast<size_t>(0), list.size());
  EXPECT_EQ(0, allocatecount);
}
