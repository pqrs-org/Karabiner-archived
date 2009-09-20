#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  ListHookedDevice::append(IOHIDevice *device)
  {
    last = device;

    for (int i = 0; ; ++i) {
      HookedDevice *p = getItem(i);
      if (p == NULL) break;

      if (! p->get()) {
        return p->initialize(device);
      }
    }
    return false;
  }

  void
  ListHookedDevice::terminate(void)
  {
    last = NULL;

    for (int i = 0; ; ++i) {
      HookedDevice *p = getItem(i);
      if (p == NULL) break;

      p->terminate();
    }
  }

  bool
  ListHookedDevice::terminate(const IOHIDevice *device)
  {
    HookedDevice *p = get(device);
    if (! p) return false;

    return p->terminate();
  }

  HookedDevice *
  ListHookedDevice::get(const IOHIDevice *device)
  {
    last = device;

    if (device == NULL) return NULL;

    for (int i = 0; ; ++i) {
      HookedDevice *p = getItem(i);
      if (p == NULL) break;

      if (p->get() == device) return p;
    }
    return NULL;
  }

  HookedDevice *
  ListHookedDevice::get(void)
  {
    HookedDevice *p = get(last);
    if (p) return p;

    for (int i = 0; ; ++i) {
      p = getItem(i);
      if (p == NULL) break;

      return p;
    }
    return NULL;
  }

  void
  ListHookedDevice::refresh(void)
  {
    for (int i = 0; ; ++i) {
      HookedDevice *p = getItem(i);
      if (p == NULL) break;

      p->refresh();
    }
  }
}
