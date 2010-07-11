#ifndef LISTHOOKEDPOINTING_HPP
#define LISTHOOKEDPOINTING_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedPointing : public ListHookedDevice {
  public:
    class Item : public ListHookedDevice::Item {
    public:
      Item(IOHIDevice* d);
      ~Item(void);

      IOHIPointing* get(void) const { return OSDynamicCast(IOHIPointing, ListHookedDevice::Item::get()); }

      RelativePointerEventCallback getOrig_relativePointerEventAction(void) const { return orig_relativePointerEventAction_; }
      ScrollWheelEventCallback getOrig_scrollWheelEventAction(void) const { return orig_scrollWheelEventAction_; }
      OSObject* getOrig_relativePointerEventTarget(void) const { return orig_relativePointerEventTarget_; }
      OSObject* getOrig_scrollWheelEventTarget(void) const { return orig_scrollWheelEventTarget_; }

      Buttons get_previousbuttons(void) const { return previousbuttons_; }
      void set_previousbuttons(Buttons newval) { previousbuttons_ = newval; }

    private:
      RelativePointerEventCallback orig_relativePointerEventAction_;
      ScrollWheelEventCallback orig_scrollWheelEventAction_;
      OSObject* orig_relativePointerEventTarget_;
      OSObject* orig_scrollWheelEventTarget_;

      Buttons previousbuttons_;

      IOLock* replacerestore_lock_;

      bool refresh_callback(void);
      bool isReplaced(void) const { return orig_relativePointerEventAction_ != NULL; }

      /** return true if event action is replaced. */
      bool replaceEventAction(void);
      /** return true if event action is replaced. */
      bool restoreEventAction(void);
    };

    static ListHookedPointing& instance(void);
    ListHookedPointing::Item* get(void) { return static_cast<ListHookedPointing::Item*>(ListHookedDevice::get()); }
    ListHookedPointing::Item* get(const IOHIPointing* pointing) { return static_cast<ListHookedPointing::Item*>(ListHookedDevice::get(pointing)); }

    static void hook_RelativePointerEventCallback_queued(Params_RelativePointerEventCallback& params);
    static void hook_ScrollWheelEventCallback_queued(Params_ScrollWheelEventCallback& params);
  };
}

#endif
