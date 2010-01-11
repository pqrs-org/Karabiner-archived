#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventWatcher.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedPointing.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    ListHookedPointing listHookedPointing;
  }

  ListHookedPointing&
  ListHookedPointing::instance(void)
  {
    return listHookedPointing;
  }

  // ----------------------------------------------------------------------
  namespace {
    void
    hook_RelativePointerEventCallback(OSObject* target,
                                      int buttons,
                                      int dx,
                                      int dy,
                                      AbsoluteTime ts,
                                      OSObject* sender,
                                      void* refcon)
    {
      if (! CommonData::eventLock) return;
      IOLockWrapper::ScopedLock lk(CommonData::eventLock);

      IOHIPointing* pointing = OSDynamicCast(IOHIPointing, sender);
      if (! pointing) return;

      HookedPointing* hp = ListHookedPointing::instance().get(pointing);
      if (! hp) return;

      // ------------------------------------------------------------
      CommonData::setcurrent_ts(ts);

      // ------------------------------------------------------------
      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, dx, dy));
      if (! ptr) return;
      Params_RelativePointerEventCallback& params = *ptr;

      if (! params.buttons.isNONE()) {
        EventWatcher::on();
      }

      Core::remap_RelativePointerEventCallback(params);
    }

    void
    hook_ScrollWheelEventCallback(OSObject* target,
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
                                  void* refcon)
    {
      if (! CommonData::eventLock) return;
      IOLockWrapper::ScopedLock lk(CommonData::eventLock);

      IOHIPointing* pointing = OSDynamicCast(IOHIPointing, sender);
      if (! pointing) return;

      HookedPointing* hp = ListHookedPointing::instance().get(pointing);
      if (! hp) return;

      // ------------------------------------------------------------
      CommonData::setcurrent_ts(ts);

      // ------------------------------------------------------------
      Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(deltaAxis1, deltaAxis2, deltaAxis3,
                                                                                           fixedDelta1, fixedDelta2, fixedDelta3,
                                                                                           pointDelta1, pointDelta2, pointDelta3,
                                                                                           options));
      if (! ptr) return;
      Params_ScrollWheelEventCallback& params = *ptr;
      // EventWatcher::on is not necessary.

      Core::remap_ScrollWheelEventCallback(params);
    }
  }

  bool
  HookedPointing::initialize(IOHIDevice* d)
  {
    if (! d) return false;

    const char* name = d->getName();
    if (! name) return false;

    if (HookedDevice::isIgnoreDevice(d)) return false;

    device_ = d;
    IOLog("KeyRemap4MacBook HookedPointing::initialize name = %s, device_ = 0x%p\n", name, device_);

    if (strcmp(name, "IOHIDPointing") == 0 ||
        strcmp(name, "AppleUSBGrIIITrackpad") == 0 ||
        strcmp(name, "AppleADBMouseType4") == 0) {
      isAppleDriver_ = true;
    } else {
      isAppleDriver_ = false;
    }

    return refresh();
  }

  bool
  HookedPointing::refresh(void)
  {
    if (! config.initialized) {
      return restoreEventAction();
    }
    if (! isAppleDriver_ && config.general_dont_remap_thirdvendor_pointing) {
      return restoreEventAction();
    }
    return replaceEventAction();
  }

  bool
  HookedPointing::terminate(void)
  {
    bool result = restoreEventAction();

    device_ = NULL;
    orig_relativePointerEventAction_ = NULL;
    orig_scrollWheelEventAction_ = NULL;
    orig_relativePointerEventTarget_ = NULL;
    orig_scrollWheelEventTarget_ = NULL;

    return result;
  }

  bool
  HookedPointing::replaceEventAction(void)
  {
    if (! device_) return false;

    IOHIPointing* pointing = OSDynamicCast(IOHIPointing, device_);
    if (! pointing) return false;

    bool result = false;

    // ----------------------------------------
    {
      RelativePointerEventCallback callback = reinterpret_cast<RelativePointerEventCallback>(pointing->_relativePointerEventAction);

      if (callback != hook_RelativePointerEventCallback) {
        IOLog("KeyRemap4MacBook HookedPointing::replaceEventAction (RelativePointerEventCallback) (device_ = 0x%p)\n", device_);

        orig_relativePointerEventAction_ = callback;
        orig_relativePointerEventTarget_ = pointing->_relativePointerEventTarget;

        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(hook_RelativePointerEventCallback);

        result = true;
      }
    }
    {
      ScrollWheelEventCallback callback = reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);

      if (callback != hook_ScrollWheelEventCallback) {
        IOLog("KeyRemap4MacBook HookedPointing::replaceEventAction (ScrollWheelEventCallback) (device_ = 0x%p)\n", device_);

        orig_scrollWheelEventAction_ = callback;
        orig_scrollWheelEventTarget_ = pointing->_scrollWheelEventTarget;

        pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(hook_ScrollWheelEventCallback);

        result = true;
      }
    }

    return result;
  }

  bool
  HookedPointing::restoreEventAction(void)
  {
    if (! device_) return false;

    IOHIPointing* pointing = OSDynamicCast(IOHIPointing, device_);
    if (! pointing) return false;

    bool result = false;

    // ----------------------------------------
    {
      RelativePointerEventCallback callback = reinterpret_cast<RelativePointerEventCallback>(pointing->_relativePointerEventAction);

      if (callback == hook_RelativePointerEventCallback) {
        IOLog("KeyRemap4MacBook HookedPointing::restoreEventAction (RelativePointerEventCallback) (device_ = 0x%p)\n", device_);

        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(orig_relativePointerEventAction_);

        result = true;
      }
    }
    {
      ScrollWheelEventCallback callback = reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);

      if (callback == hook_ScrollWheelEventCallback) {
        IOLog("KeyRemap4MacBook HookedPointing::restoreEventAction (ScrollWheelEventCallback) (device_ = 0x%p)\n", device_);

        pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(orig_scrollWheelEventAction_);

        result = true;
      }
    }

    orig_relativePointerEventAction_ = NULL;
    orig_scrollWheelEventAction_ = NULL;
    orig_relativePointerEventTarget_ = NULL;
    orig_scrollWheelEventTarget_ = NULL;

    return result;
  }
}
