#include <IOKit/hid/IOHIDKeys.h>
#include "ListHookedDevice.hpp"
#include "NumHeldDownKeys.hpp"
#include "IOLockWrapper.hpp"

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
    lock_ = IOLockWrapper::alloc();
    if (! lock_) return false;

    return true;
  }

  bool
  ListHookedDevice::append(IOHIDevice* device)
  {
    if (! lock_) return false;
    IOLockWrapper::ScopedLock lk(lock_);

    last_ = device;

    for (int i = 0; i < MAXNUM; ++i) {
      HookedDevice* p = getItem(i);
      if (! p) continue;
      if (p->get()) continue;

      IOLog("KeyRemap4MacBook ListHookedDevice::append (device = 0x%p, slot = %d)\n", device, i);

      bool result = p->initialize(device);
      if (result) {
        // reset if any event actions are replaced.
        reset();
      }
      return reset;
    }

    return false;
  }

  void
  ListHookedDevice::terminate(void)
  {
    if (! lock_) return;

    {
      IOLockWrapper::ScopedLock lk(lock_);

      last_ = NULL;

      for (int i = 0; i < MAXNUM; ++i) {
        HookedDevice* p = getItem(i);
        if (! p) continue;

        p->terminate();
      }
    }

    reset();

    IOLockWrapper::free(lock_);
  }

  bool
  ListHookedDevice::terminate(const IOHIDevice* device)
  {
    if (! lock_) return false;
    IOLockWrapper::ScopedLock lk(lock_);

    HookedDevice* p = get_nolock(device);
    if (! p) return false;

    bool result = p->terminate();
    if (! result) return false;

    reset();
    return true;
  }

  HookedDevice*
  ListHookedDevice::get_nolock(const IOHIDevice* device)
  {
    last_ = device;

    if (! device) return NULL;

    for (int i = 0; i < MAXNUM; ++i) {
      HookedDevice* p = getItem(i);
      if (! p) continue;

      if (p->get() == device) return p;
    }
    return NULL;
  }

  HookedDevice*
  ListHookedDevice::get(const IOHIDevice* device)
  {
    if (! lock_) return NULL;
    IOLockWrapper::ScopedLock lk(lock_);

    return get_nolock(device);
  }

  HookedDevice*
  ListHookedDevice::get(void)
  {
    if (! lock_) return NULL;
    IOLockWrapper::ScopedLock lk(lock_);

    // ----------------------------------------------------------------------
    HookedDevice* p = get_nolock(last_);
    if (p) return p;

    for (int i = 0; i < MAXNUM; ++i) {
      p = getItem(i);
      if (p) return p;
    }

    return NULL;
  }

  void
  ListHookedDevice::refresh(void)
  {
    if (! lock_) return;
    IOLockWrapper::ScopedLock lk(lock_);

    for (int i = 0; i < MAXNUM; ++i) {
      HookedDevice* p = getItem(i);
      if (! p) continue;

      if (p->refresh()) {
        // reset if any event actions are replaced.
        reset();
      }
    }
  }
}
