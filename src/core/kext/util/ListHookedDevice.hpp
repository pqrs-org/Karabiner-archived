#ifndef LISTHOOKEDDEVICE_HPP
#define LISTHOOKEDDEVICE_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedDevice {
  public:
    HookedDevice(void) : device(NULL) {}

    virtual bool initialize(IOHIDevice *_device) = 0;
    virtual bool refresh(void) = 0;
    virtual bool terminate(void) = 0;

    IOHIDevice *get(void) const { return device; }

  protected:
    IOHIDevice *device;
  };

  class ListHookedDevice {
  public:
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
    ListHookedDevice(void) : last(NULL) {}
    virtual ~ListHookedDevice(void) {}

  private:
    virtual HookedDevice *getItem(int index) = 0;
    const IOHIDevice *last;
  };
}

#endif
