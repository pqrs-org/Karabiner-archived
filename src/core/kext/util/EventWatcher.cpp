#include "Config.hpp"
#include "EventWatcher.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool* EventWatcher::item_[MAXNUM];
  IOLock* EventWatcher::lock_;

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
      item_[i] = NULL;
    }
  }

  void
  EventWatcher::on(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (config.debug_devel) {
      IOLog("KeyRemap4MacBook -Info- EventWatcher::on\n");
    }

    for (int i = 0; i < MAXNUM; ++i) {
      if (item_[i]) {
        *(item_[i]) = true;
        item_[i] = NULL;
      }
    }
  }

  void
  EventWatcher::set(bool& b)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    b = false;
    for (int i = 0; i < MAXNUM; ++i) {
      if (item_[i] == NULL) {
        item_[i] = &b;

        if (i == MAXNUM - 1) {
          IOLog("KeyRemap4MacBook --Warning-- EventWatcher was filled up. Expand MAXNUM.\n");
        }
        return;
      }
    }
  }

  void
  EventWatcher::unset(bool& b)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    for (int i = 0; i < MAXNUM; ++i) {
      if (item_[i] == &b) {
        item_[i] = NULL;
      }
    }
  }
}
