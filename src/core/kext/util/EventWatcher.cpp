#include "EventWatcher.hpp"
#include "RemapUtil.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace EventWatcher {
    enum {
      MAXNUM = 32,
    };
    bool* item[MAXNUM];
    IOLock* lock = NULL;

    void
    initialize(void)
    {
      lock = IOLockWrapper::alloc();
      reset();
    }

    void
    terminate(void)
    {
      IOLockWrapper::free(lock);
    }

    void
    reset(void)
    {
      IOLockWrapper::ScopedLock lk(lock);

      for (int i = 0; i < MAXNUM; ++i) {
        item[i] = NULL;
      }
    }

    void
    on(void)
    {
      IOLockWrapper::ScopedLock lk(lock);

      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i]) {
          *(item[i]) = true;
          item[i] = NULL;
        }
      }
    }

    void
    set(bool* b)
    {
      IOLockWrapper::ScopedLock lk(lock);

      if (b == NULL) return;

      *b = false;
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i] == NULL) {
          item[i] = b;
        }
      }
    }

    void
    unset(bool* b)
    {
      IOLockWrapper::ScopedLock lk(lock);

      if (b == NULL) return;

      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i] == b) {
          item[i] = NULL;
        }
      }
    }
  }
}
