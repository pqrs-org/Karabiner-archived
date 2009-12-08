#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventWatcher.hpp"
#include "ListHookedPointing.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    ListHookedPointing listHookedPointing;
  }

  ListHookedPointing &
  ListHookedPointing::instance(void)
  {
    return listHookedPointing;
  }

  // ----------------------------------------------------------------------
  namespace {
    void
    hook_RelativePointerEventCallback(OSObject *target,
                                      int buttons,
                                      int dx,
                                      int dy,
                                      AbsoluteTime ts,
                                      OSObject *sender,
                                      void *refcon)
    {
      IOHIPointing* pointing = OSDynamicCast(IOHIPointing, sender);
      if (! pointing) return;

      HookedPointing* hp = ListHookedPointing::instance().get(pointing);
      if (! hp) return;

      // ------------------------------------------------------------
      EventWatcher::countup();

      Params_RelativePointerEventCallback params(buttons, dx, dy);
      CommonData::setcurrent_ts(ts);

      Core::remap_RelativePointerEventCallback(params);
    }

    void
    hook_ScrollWheelEventCallback(OSObject * target,
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
                                  OSObject *sender,
                                  void *refcon)
    {
      IOHIPointing* pointing = OSDynamicCast(IOHIPointing, sender);
      if (! pointing) return;

      HookedPointing* hp = ListHookedPointing::instance().get(pointing);
      if (! hp) return;

      // ------------------------------------------------------------
      EventWatcher::countup();

      Params_ScrollWheelEventCallback params(deltaAxis1, deltaAxis2, deltaAxis3,
                                             fixedDelta1, fixedDelta2, fixedDelta3,
                                             pointDelta1, pointDelta2, pointDelta3,
                                             options);
      CommonData::setcurrent_ts(ts);

      Core::remap_ScrollWheelEventCallback(params);
    }
  }

  HookedPointing::HookedPointing(void) :
    isAppleDriver(false),
    orig_relativePointerEventAction(NULL), orig_scrollWheelEventAction(NULL),
    orig_relativePointerEventTarget(NULL), orig_scrollWheelEventTarget(NULL)
  {
  }

  bool
  HookedPointing::initialize(IOHIDevice *_device)
  {
    const char *name = _device->getName();
    if (HookedDevice::isIgnoreDevice(_device)) return false;

    device = _device;
    IOLog("KeyRemap4MacBook HookedPointing::initialize name = %s, device = 0x%p\n", name, device);

    if (strcmp(name, "IOHIDPointing") == 0 ||
        strcmp(name, "AppleUSBGrIIITrackpad") == 0 ||
        strcmp(name, "AppleADBMouseType4") == 0) {
      isAppleDriver = true;
    } else {
      isAppleDriver = false;
    }

    return refresh();
  }

  bool
  HookedPointing::refresh(void)
  {
    if (! config.initialized) {
      return restoreEventAction();
    }
    if (! isAppleDriver && config.general_dont_remap_thirdvendor_pointing) {
      return restoreEventAction();
    }
    return replaceEventAction();
  }

  bool
  HookedPointing::terminate(void)
  {
    bool result = restoreEventAction();

    device = NULL;
    orig_relativePointerEventAction = NULL;
    orig_scrollWheelEventAction = NULL;
    orig_relativePointerEventTarget = NULL;
    orig_scrollWheelEventTarget = NULL;

    return result;
  }

  bool
  HookedPointing::replaceEventAction(void)
  {
    if (! device) return false;

    IOHIPointing *pointing = OSDynamicCast(IOHIPointing, device);
    if (! pointing) return false;

    bool result = false;

    // ----------------------------------------
    {
      RelativePointerEventCallback callback = reinterpret_cast<RelativePointerEventCallback>(pointing->_relativePointerEventAction);

      if (callback != hook_RelativePointerEventCallback) {
        IOLog("KeyRemap4MacBook HookedPointing::replaceEventAction (RelativePointerEventCallback) (device = 0x%p)\n", device);

        orig_relativePointerEventAction = callback;
        orig_relativePointerEventTarget = pointing->_relativePointerEventTarget;

        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(hook_RelativePointerEventCallback);

        result = true;
      }
    }
    {
      ScrollWheelEventCallback callback = reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);

      if (callback != hook_ScrollWheelEventCallback) {
        IOLog("KeyRemap4MacBook HookedPointing::replaceEventAction (ScrollWheelEventCallback) (device = 0x%p)\n", device);

        orig_scrollWheelEventAction = callback;
        orig_scrollWheelEventTarget = pointing->_scrollWheelEventTarget;

        pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(hook_ScrollWheelEventCallback);

        result = true;
      }
    }

    return result;
  }

  bool
  HookedPointing::restoreEventAction(void)
  {
    if (! device) return false;

    IOHIPointing *pointing = OSDynamicCast(IOHIPointing, device);
    if (! pointing) return false;

    bool result = false;

    // ----------------------------------------
    {
      RelativePointerEventCallback callback = reinterpret_cast<RelativePointerEventCallback>(pointing->_relativePointerEventAction);

      if (callback == hook_RelativePointerEventCallback) {
        IOLog("KeyRemap4MacBook HookedPointing::restoreEventAction (RelativePointerEventCallback) (device = 0x%p)\n", device);

        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(orig_relativePointerEventAction);

        result = true;
      }
    }
    {
      ScrollWheelEventCallback callback = reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);

      if (callback == hook_ScrollWheelEventCallback) {
        IOLog("KeyRemap4MacBook HookedPointing::restoreEventAction (ScrollWheelEventCallback) (device = 0x%p)\n", device);

        pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(orig_scrollWheelEventAction);

        result = true;
      }
    }

    orig_relativePointerEventAction = NULL;
    orig_scrollWheelEventAction = NULL;
    orig_relativePointerEventTarget = NULL;
    orig_scrollWheelEventTarget = NULL;

    return result;
  }
}
