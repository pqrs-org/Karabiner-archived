#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  ListHookedDevice::initialize(void)
  {
    lock = IOLockAlloc();
    if (! lock) {
      IOLog("[KeyRemap4MacBook WARNING] ListHookedDevice::initialize IOLockAlloc failed.\n");
      return false;
    }
    return true;
  }

  bool
  ListHookedDevice::append(IOHIDevice *device)
  {
    if (! lock) return false;

    // ------------------------------------------------------------
    bool result = false;

    IOLockLock(lock);
    {
      last = device;

      for (int i = 0; i < MAXNUM; ++i) {
        HookedDevice *p = getItem(i);
        if (! p) continue;

        if (! p->get()) {
          result = p->initialize(device);
          break;
        }
      }
    }
    IOLockUnlock(lock);

    return result;
  }

  void
  ListHookedDevice::terminate(void)
  {
    if (! lock) return;

    // ------------------------------------------------------------
    IOLock *l = NULL;
    if (lock) {
      l = lock;
      IOLockLock(l);
      lock = NULL;
    }

    {
      // lock scope
      last = NULL;

      for (int i = 0; i < MAXNUM; ++i) {
        HookedDevice *p = getItem(i);
        if (! p) continue;

        p->terminate();
      }
    }

    // ----------------------------------------
    if (l) {
      IOLockUnlock(l);
      IOLockFree(l);
    }
  }

  bool
  ListHookedDevice::terminate(const IOHIDevice *device)
  {
    if (! lock) return false;

    // ----------------------------------------------------------------------
    bool result = false;

    IOLockLock(lock);
    {
      HookedDevice *p = get_nolock(device);
      if (p) {
        result = p->terminate();
      }
    }
    IOLockUnlock(lock);

    return result;
  }

  HookedDevice *
  ListHookedDevice::get_nolock(const IOHIDevice *device)
  {
    last = device;

    if (! device) return NULL;

    for (int i = 0; i < MAXNUM; ++i) {
      HookedDevice *p = getItem(i);
      if (! p) continue;

      if (p->get() == device) return p;
    }
    return NULL;
  }


  HookedDevice *
  ListHookedDevice::get(const IOHIDevice *device)
  {
    if (! lock) return NULL;

    // ----------------------------------------------------------------------
    HookedDevice *result = NULL;

    IOLockLock(lock);
    {
      result = get_nolock(device);
    }
    IOLockUnlock(lock);

    return result;
  }

  HookedDevice *
  ListHookedDevice::get(void)
  {
    if (! lock) return NULL;

    // ----------------------------------------------------------------------
    HookedDevice *result = NULL;

    IOLockLock(lock);
    {
      result = get_nolock(last);

      if (! result) {
        for (int i = 0; i < MAXNUM; ++i) {
          result = getItem(i);
          if (result) break;
        }
      }
    }
    IOLockUnlock(lock);

    return result;
  }

  void
  ListHookedDevice::refresh(void)
  {
    if (! lock) return;

    // ----------------------------------------------------------------------
    IOLockLock(lock);
    {
      for (int i = 0; i < MAXNUM; ++i) {
        HookedDevice *p = getItem(i);
        if (! p) continue;

        p->refresh();
      }
    }
    IOLockUnlock(lock);
  }
}
