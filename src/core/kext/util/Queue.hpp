#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class Queue {
  public:
    class Item {
      friend class Queue;

    public:
      Item(void) : prev_(NULL), next_(NULL) {}
      virtual ~Item(void) {}

      Item* getprev(void) { return prev_; }
      Item* getnext(void) { return next_; }

    protected:
      Item* prev_;
      Item* next_;
    };

    Queue(void);
    ~Queue(void);

    Item* front(void) { return front_; }
    Item* back(void) { return back_; }
    bool empty(void) { return front_ == NULL; }

    // ----------------------------------------
    // *** Don't push same item twice. ***
    // *** BAD EXAMPLE ***
    // Queue::Item item;
    // queue.push(&item);
    // queue.push(&item); // broken!!!
    //
    // ----------------------------------------
    // You need to delete item by yourself.
    //
    // Example:
    //
    //   queue.push(new Queue::Item);
    //
    //   Queue::Item* p = queue.front();
    //   queue.pop();
    //   delete p;
    //
    void push(Item* item);
    void pop(void);

  private:
    Item* front_;
    Item* back_;

    IOLock* lock_;
  };
}

#endif
