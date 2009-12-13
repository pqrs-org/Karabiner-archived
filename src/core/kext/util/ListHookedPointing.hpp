#ifndef LISTHOOKEDPOINTING_HPP
#define LISTHOOKEDPOINTING_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedPointing : public HookedDevice {
  public:
    HookedPointing(void);

    IOHIPointing* get(void) const { return OSDynamicCast(IOHIPointing, HookedDevice::get()); }

    RelativePointerEventCallback getOrig_relativePointerEventAction() const { return orig_relativePointerEventAction_; }
    ScrollWheelEventCallback getOrig_scrollWheelEventAction() const { return orig_scrollWheelEventAction_; }
    OSObject* getOrig_relativePointerEventTarget() const { return orig_relativePointerEventTarget_; }
    OSObject* getOrig_scrollWheelEventTarget() const { return orig_scrollWheelEventTarget_; }

  private:
    bool isAppleDriver_;

    RelativePointerEventCallback orig_relativePointerEventAction_;
    ScrollWheelEventCallback orig_scrollWheelEventAction_;
    OSObject* orig_relativePointerEventTarget_;
    OSObject* orig_scrollWheelEventTarget_;

    bool initialize(IOHIDevice* d);
    bool refresh(void);
    bool terminate(void);

    /** return true if event action is replaced. */
    bool replaceEventAction(void);
    /** return true if event action is replaced. */
    bool restoreEventAction(void);
  };

  class ListHookedPointing : public ListHookedDevice {
  public:
    static ListHookedPointing& instance(void);
    HookedPointing* get(void) { return static_cast<HookedPointing*>(ListHookedDevice::get()); }
    HookedPointing* get(const IOHIPointing* pointing) { return static_cast<HookedPointing*>(ListHookedDevice::get(pointing)); }

  private:
    HookedPointing item_[MAXNUM];

    HookedDevice* getItem(int index) {
      if (index < 0 || index >= MAXNUM) return NULL;
      return item_ + index;
    }
  };
}

#endif
