#ifndef LISTHOOKEDPOINTING_HPP
#define LISTHOOKEDPOINTING_HPP

#include "base.hpp"
#include "CallBackWrapper.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedPointing : public ListHookedDevice {
  public:
    class Item : public ListHookedDevice::Item {
    public:
      Item(IOHIDevice* d);
      ~Item(void);

      Buttons get_previousbuttons(void) const { return previousbuttons_; }
      void set_previousbuttons(Buttons newval) { previousbuttons_ = newval; }

      void apply(const Params_RelativePointerEventCallback& params);
      void apply(const Params_ScrollWheelEventCallback& params);

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

    static void hook_RelativePointerEventCallback_queued(Params_RelativePointerEventCallback& params);
    static void hook_ScrollWheelEventCallback_queued(Params_ScrollWheelEventCallback& params);

    void apply(const Params_RelativePointerEventCallback& params);
    void apply(const Params_ScrollWheelEventCallback& params);

  private:
    static void hook_RelativePointerEventCallback(OSObject* target,
                                                  int buttons,
                                                  int dx,
                                                  int dy,
                                                  AbsoluteTime ts,
                                                  OSObject* sender,
                                                  void* refcon);
    static void hook_ScrollWheelEventCallback(OSObject* target,
                                              short deltaAxis1,
                                              short deltaAxis2,
                                              short deltaAxis3,
                                              IOFixed fixedDelta1,
                                              IOFixed fixedDelta2,
                                              IOFixed fixedDelta3,
                                              SInt32 pointDelta1,
                                              SInt32 pointDelta2,
                                              SInt32 pointDelta3,
                                              SInt32 options,
                                              AbsoluteTime ts,
                                              OSObject* sender,
                                              void* refcon);
  };
}

#endif
