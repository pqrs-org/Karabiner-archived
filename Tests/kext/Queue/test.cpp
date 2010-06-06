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
  const int MAXITEM = 10;

  for (int i = 0; i < MAXITEM; ++i) {
    queue.push(new TestItem(i));
  }

  for (int i = 0; i < MAXITEM; ++i) {
    TestItem* p = dynamic_cast<TestItem*>(queue.front());
    queue.pop();
    EXPECT_TRUE(p != NULL);

    EXPECT_EQ(i, p->v());
    delete p;
  }
}
