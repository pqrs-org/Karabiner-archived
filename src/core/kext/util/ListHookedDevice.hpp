#ifndef LISTHOOKEDDEVICE_HPP
#define LISTHOOKEDDEVICE_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedDevice {
    friend class ListHookedDevice;

  public:
    HookedDevice(void) : device(NULL) {}

    IOHIDevice *get(void) const { return device; }

  protected:
    IOHIDevice *device;

    virtual bool initialize(IOHIDevice *_device) = 0;
    virtual bool refresh(void) = 0;
    virtual bool terminate(void) = 0;
  };

  class ListHookedDevice {
  public:
    bool initialize(void);
    bool append(IOHIDevice *device);
    void terminate(void);
    bool terminate(const IOHIDevice *device);

    HookedDevice *get(const IOHIDevice *device);
    HookedDevice *get(void);
    void refresh(void);

  protected:
    enum {
      MAXNUM = 16,
    };
    ListHookedDevice(void) : last(NULL), lock(NULL) {}
    virtual ~ListHookedDevice(void) {}

  private:
    virtual HookedDevice *getItem(int index) = 0;
    HookedDevice *get_nolock(const IOHIDevice *device);

    const IOHIDevice *last;

    IOLock *lock;
  };
}

#endif
