#include "Queue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Queue::Queue(void) : front_(NULL), back_(NULL)
  {
    lock_ = IOLockWrapper::alloc();
  }

  Queue::~Queue(void)
  {
    IOLockWrapper::free(lock_);
  }

  void
  Queue::push(Item* item)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (! item) return;

    if (back_) {
      back_->next_ = item;
    }
    item->prev_ = back_;
    item->next_ = NULL;

    back_ = item;

    if (! front_) {
      front_ = back_;
    }
  }

  void
  Queue::pop(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (! front_) return;

    Item* next = front_->next_;
    front_->next_ = NULL;
    front_ = next;

    if (next) {
      next->prev_ = NULL;
    } else {
      back_ = NULL;
    }
  }
}
