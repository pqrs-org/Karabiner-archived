#ifndef LISTHOOKEDPOINTING_HPP
#define LISTHOOKEDPOINTING_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedPointing : public HookedDevice {
  public:
    HookedPointing(void);

    IOHIPointing *get(void) const { return OSDynamicCast(IOHIPointing, HookedDevice::get()); }

    RelativePointerEventCallback getOrig_relativePointerEventAction() const { return orig_relativePointerEventAction; }
    ScrollWheelEventCallback getOrig_scrollWheelEventAction() const { return orig_scrollWheelEventAction; }
    OSObject *getOrig_relativePointerEventTarget() const { return orig_relativePointerEventTarget; }
    OSObject *getOrig_scrollWheelEventTarget() const { return orig_scrollWheelEventTarget; }

  private:
    bool isAppleDriver;

    RelativePointerEventCallback orig_relativePointerEventAction;
    ScrollWheelEventCallback orig_scrollWheelEventAction;
    OSObject *orig_relativePointerEventTarget;
    OSObject *orig_scrollWheelEventTarget;

    bool initialize(IOHIDevice *_device);
    bool refresh(void);
    bool terminate(void);

    bool replaceEventAction(void);
    bool restoreEventAction(void);
  };

  class ListHookedPointing : public ListHookedDevice {
  public:
    static ListHookedPointing &instance(void);
    HookedPointing *get(void) { return static_cast<HookedPointing *>(ListHookedDevice::get()); }
    HookedPointing *get(const IOHIPointing *pointing) { return static_cast<HookedPointing *>(ListHookedDevice::get(pointing)); }

  private:
    HookedPointing item[MAXNUM];

    HookedDevice *getItem(int index) {
      if (index < 0 || index >= MAXNUM) return NULL;
      return item + index;
    }
  };
}

#endif
