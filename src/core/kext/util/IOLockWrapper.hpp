#ifndef LOCKWRAPPER_HPP
#define LOCKWRAPPER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class IOLockWrapper {
  public:
    static IOLock* alloc(void) {
      IOLock* lock = IOLockAlloc();
      if (! lock) {
        IOLog("[KeyRemap4MacBook ERROR] IOLockAlloc failed.\n");
      }
      return lock;
    }
    static void free(IOLock*& lock) {
      IOLockLock(lock);
      IOLock *tmplock = lock;
      lock = NULL;
      IOLockUnlock(tmplock);
      IOLockFree(tmplock);
    }

    class ScopedLock {
    public:
      // ------------------------------------------------------------
      ScopedLock(IOLock* lock) : lock_(lock) {
        if (lock_) {
          IOLockLock(lock_);
        }
      }
      ~ScopedLock(void) {
        if (lock_) {
          IOLockUnlock(lock_);
        }
      }

    private:
      IOLock* lock_;
    };
  };
}

#endif
