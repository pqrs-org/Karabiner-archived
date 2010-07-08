#ifndef LIST_HPP
#define LIST_HPP

#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class List {
  public:
    class Item {
      friend class List;

    public:
      Item(void) : prev_(NULL), next_(NULL) {}
      virtual ~Item(void) {}

      Item* getprev(void) { return prev_; }
      Item* getnext(void) { return next_; }

    protected:
      Item* prev_;
      Item* next_;
    };

    List(void);
    ~List(void);

    Item* front(void) { return front_; }
    Item* back(void) { return back_; }
    bool empty(void) const { return front_ == NULL; }
    size_t size(void) const { return size_; }

    Item* erase(Item* p);
    void clear(void);

    // We must call push_back with Item which allocated by "new".
    // Item will be deleted by pop/erase, you must not delete it yourself.
    //
    // Example:
    //   queue.push_back(new List::Item);
    //
    //   List::Item* p = queue.front();
    //   queue.pop_front();
    //
    // ----------------------------------------
    // *** Don't push same item twice. ***
    // *** BAD EXAMPLE ***
    // List::Item* item = new List::Item();
    // queue.push_back(item);
    // queue.push_back(item); // broken!!!
    //
    // ----------------------------------------
    void push_back(Item* p);
    void pop_front(void) { erase(front_); }

  private:
    Item* erase_nolock(Item* p);

    Item* front_;
    Item* back_;
    size_t size_;

    IOLock* lock_;
  };
}

#endif
