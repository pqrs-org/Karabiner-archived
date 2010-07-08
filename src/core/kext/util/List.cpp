#include "List.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List::List(void) : front_(NULL), back_(NULL), size_(0)
  {
    lock_ = IOLockWrapper::alloc();
  }

  List::~List(void)
  {
    clear();
    IOLockWrapper::free(lock_);
  }

  List::Item*
  List::erase_nolock(Item* p)
  {
    if (! p) return NULL;

    Item* next = p->next_;

    if (p->prev_) {
      p->prev_->next_ = p->next_;
    }
    if (p->next_) {
      p->next_->prev_ = p->prev_;
    }
    if (front_ == p) {
      front_ = p->next_;
    }
    if (back_ == p) {
      back_ = p->prev_;
    }

    delete p;

    --size_;
    return next;
  }

  List::Item*
  List::erase(Item* p)
  {
    IOLockWrapper::ScopedLock lk(lock_);
    return erase_nolock(p);
  }

  void
  List::clear(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);
    while (front_) {
      erase_nolock(front_);
    }
  }

  void
  List::push_back(Item* p)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (! p) return;

    if (back_) {
      back_->next_ = p;
    }
    p->prev_ = back_;
    p->next_ = NULL;

    back_ = p;

    if (! front_) {
      front_ = back_;
    }

    ++size_;
  }
}
