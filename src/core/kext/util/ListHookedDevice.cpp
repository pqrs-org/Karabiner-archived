#include <IOKit/hid/IOHIDKeys.h>
#include "ListHookedDevice.hpp"
#include "NumHeldDownKeys.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    void
    reset(void)
    {
      NumHeldDownKeys::reset();
    }
  }

  bool
  HookedDevice::isIgnoreDevice(IOHIDevice* dev)
  {
    if (! dev) return false;

    // ------------------------------------------------------------
    const OSNumber* number = NULL;

    number = OSDynamicCast(OSNumber, dev->getProperty(kIOHIDVendorIDKey));
    if (! number) return false;
    UInt32 vendorID = number->unsigned32BitValue();

    number = OSDynamicCast(OSNumber, dev->getProperty(kIOHIDProductIDKey));
    if (! number) return false;
    UInt32 productID = number->unsigned32BitValue();

    IOLog("KeyRemap4MacBook HookedDevice::isIgnoreDevice checking vendorID = 0x%x, productID = 0x%x\n",
          static_cast<unsigned int>(vendorID),
          static_cast<unsigned int>(productID));

    // ------------------------------------------------------------
    // Logitech USB Headset
    if (vendorID == 0x046d && productID == 0x0a0b) return true;

#if 0
    // Apple External Keyboard
    if (vendorID == 0x05ac && productID == 0x0222) return true;

    // My Private Mouse (SIGMA Levy)
    if (vendorID == 0x093a && productID == 0x2510) return true;
#endif

    return false;
  }

  // ----------------------------------------------------------------------
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
          IOLog("KeyRemap4MacBook ListHookedDevice::append (device = 0x%p, slot = %d)\n", device, i);

          result = p->initialize(device);
          if (result) {
            // reset if any event actions are replaced.
            reset();
          }
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

    reset();

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
        if (result) {
          reset();
        }
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

        if (p->refresh()) {
          // reset if any event actions are replaced.
          reset();
        }
      }
    }
    IOLockUnlock(lock);
  }
}
