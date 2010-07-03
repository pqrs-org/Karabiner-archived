#include <ostream>
#include <gtest/gtest.h>
#include "Queue.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

class TestItem : public Queue::Item {
public:
  TestItem(int v) : value_(v) {}
  virtual ~TestItem(void) {
    //std::cout << "~TestItem(void);" << std::endl;
  };

  int v(void) { return value_; }

private:
  int value_;
};

TEST(Queue, push) {
  Queue queue;
  TestItem* p = NULL;
  const int MAXITEM = 10;

  // ------------------------------------------------------------
  EXPECT_EQ(static_cast<size_t>(0), queue.size());
  for (int i = 0; i < MAXITEM; ++i) {
    queue.push(new TestItem(i));
    EXPECT_EQ(static_cast<size_t>(i + 1), queue.size());
  }

  for (int i = 0; i < MAXITEM; ++i) {
    p = dynamic_cast<TestItem*>(queue.front());
    queue.pop();
    EXPECT_TRUE(p != NULL);

    EXPECT_EQ(i, p->v());
    EXPECT_EQ(static_cast<size_t>(MAXITEM - (i + 1)), queue.size());
    delete p;
  }
  EXPECT_EQ(static_cast<size_t>(0), queue.size());

  // ------------------------------------------------------------
  // check prev/next value.
  TestItem item1(1);
  TestItem item2(2);
  EXPECT_TRUE(item1.getprev() == NULL);
  EXPECT_TRUE(item1.getnext() == NULL);
  EXPECT_TRUE(item2.getprev() == NULL);
  EXPECT_TRUE(item2.getnext() == NULL);

  queue.push(&item1);
  EXPECT_TRUE(item1.getprev() == NULL);
  EXPECT_TRUE(item1.getnext() == NULL);
  queue.push(&item2);
  EXPECT_TRUE(item1.getprev() == NULL);
  EXPECT_TRUE(item1.getnext() != NULL);
  EXPECT_TRUE(item2.getprev() != NULL);
  EXPECT_TRUE(item2.getnext() == NULL);

  queue.pop();
  EXPECT_TRUE(item1.getprev() == NULL);
  EXPECT_TRUE(item1.getnext() == NULL);
  EXPECT_TRUE(item2.getprev() == NULL);
  EXPECT_TRUE(item2.getnext() == NULL);

  queue.pop();
}

TEST(Queue, pop) {
  Queue queue;
  queue.pop();
}
