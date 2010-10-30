#include "Config.hpp"
#include "EventWatcher.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* EventWatcher::list_;
  IOLock* EventWatcher::lock_;

  void
  EventWatcher::initialize(void)
  {
    lock_ = IOLockWrapper::alloc();
    list_ = new List();
  }

  void
  EventWatcher::terminate(void)
  {
    {
      IOLockWrapper::ScopedLock lk(lock_);
      if (list_) {
        delete list_;
      }
    }
    IOLockWrapper::free(lock_);
  }

  void
  EventWatcher::reset(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);
    if (! lk) return;

    if (! list_) return;

    list_->clear();
  }

  void
  EventWatcher::on(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);
    if (! lk) return;

    if (! list_) return;

    IOLOG_DEVEL("EventWatcher::on (list_->size:%d)\n", static_cast<int>(list_->size()));

    for (Item* p = static_cast<Item*>(list_->front()); p; p = static_cast<Item*>(p->getnext())) {
      p->isAnyEventHappen = true;
    }
  }

  void
  EventWatcher::set(bool& b)
  {
    IOLockWrapper::ScopedLock lk(lock_);
    if (! lk) return;

    if (! list_) return;

    b = false;
    list_->push_back(new Item(b));
  }

  void
  EventWatcher::unset(bool& b)
  {
    IOLockWrapper::ScopedLock lk(lock_);
    if (! lk) return;

    if (! list_) return;

    Item* p = static_cast<Item*>(list_->front());
    for (;;) {
      if (! p) break;

      if (&(p->isAnyEventHappen) == &b) {
        p = static_cast<Item*>(list_->erase(p));
      } else {
        p = static_cast<Item*>(p->getnext());
      }
    }
  }
}
