#include "EventWatcher.hpp"
#include "RemapUtil.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  EventWatcher::Item EventWatcher::item_[MAXNUM];
  IOLock* EventWatcher::lock_;
  int EventWatcher::count_;

  void
  EventWatcher::initialize(void)
  {
    lock_ = IOLockWrapper::alloc();
    reset();
  }

  void
  EventWatcher::terminate(void)
  {
    IOLockWrapper::free(lock_);
  }

  void
  EventWatcher::reset(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].flag = NULL;
    }
  }

  void
  EventWatcher::on(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    for (int i = 0; i < MAXNUM; ++i) {
      if (item_[i].flag && item_[i].count != count_) {
        *(item_[i].flag) = true;
        item_[i].flag = NULL;
      }
    }
  }

  void
  EventWatcher::set(bool& b)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    b = false;
    for (int i = 0; i < MAXNUM; ++i) {
      if (item_[i].flag == NULL) {
        item_[i].flag = &b;
        item_[i].count = count_;
        return;
      }
    }
  }

  void
  EventWatcher::unset(bool& b)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    for (int i = 0; i < MAXNUM; ++i) {
      if (item_[i].flag == &b) {
        item_[i].flag = NULL;
      }
    }
  }

  void
  EventWatcher::countup(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    ++count_;
  }
}
