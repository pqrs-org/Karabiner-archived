#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventWatcher.hpp"
#include "FlagStatus.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedPointing.hpp"
#include "NumHeldDownKeys.hpp"

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
      CommonData::setcurrent_vendorIDproductID(hp->getVendorID(), hp->getProductID());

      // ------------------------------------------------------------
      // clear temporary_count_
      if (! config.general_lazy_modifiers_with_mouse_event) {
        FlagStatus::set();
      }
      int diff = ButtonStatus::set(buttons, hp->get_previousbuttons());
      hp->set_previousbuttons(buttons);
      NumHeldDownKeys::set(diff);

      if (ButtonStatus::justPressed() != Buttons(0)) {
        CommonData::setcurrent_workspacedata();
      }

      // ------------------------------------------------------------
      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, dx, dy));
      if (! ptr) return;
      Params_RelativePointerEventCallback& params = *ptr;

      // We set EventWatcher::on only when Buttons pressed.
      // It's cause a problem when you use the following settings. (Unexpected FN_Lock is fired).
      //   - FN+CursorMove to ScrollWheel
      //   - FN to FN (+ When you type FN only, send FN_Lock)
      //
      // But, if we call EventWatcher::on every CursorMove event, unexpected cancel occurs.
      // It's more terrible than above problem.
      // So, we keep to call EventWatcher::on only when Buttons pressed.
      if (ButtonStatus::justPressed() != Buttons(0)) {
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
      CommonData::setcurrent_vendorIDproductID(hp->getVendorID(), hp->getProductID());

      // ------------------------------------------------------------
      // clear temporary_count_
      if (! config.general_lazy_modifiers_with_mouse_event) {
        FlagStatus::set();
      }

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

    device_ = d;
    previousbuttons_ = 0;
    IOLOG_INFO("HookedPointing::initialize name:%s, device_:%p\n", name, device_);

    return refresh();
  }

  bool
  HookedPointing::refresh(void)
  {
    if (! config.initialized) {
      goto restore;
    }
    if (config.general_dont_remap_thirdvendor_pointing &&
        deviceType_ != DeviceType::APPLE_INTERNAL &&
        deviceType_ != DeviceType::APPLE_EXTERNAL) {
      goto restore;
    }
    // Logitech Cordless Presenter
    if (config.general_dont_remap_logitech_cordless_presenter &&
        isEqualVendorIDProductID(DeviceVendorID(0x046d), DeviceProductID(0xc515))) {
      goto restore;
    }
#if 0
    // Apple Magic Mouse
    if (isEqualVendorIDProductID(DeviceVendorID(0x05ac), DeviceProductID(0x030d))) {
      goto restore;
    }
#endif

    return replaceEventAction();

  restore:
    return restoreEventAction();
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
        IOLOG_INFO("HookedPointing::replaceEventAction (RelativePointerEventCallback) device_:%p\n", device_);

        orig_relativePointerEventAction_ = callback;
        orig_relativePointerEventTarget_ = pointing->_relativePointerEventTarget;

        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(hook_RelativePointerEventCallback);

        result = true;
      }
    }
    {
      ScrollWheelEventCallback callback = reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);

      if (callback != hook_ScrollWheelEventCallback) {
        IOLOG_INFO("HookedPointing::replaceEventAction (ScrollWheelEventCallback) device_:%p\n", device_);

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
        IOLOG_INFO("HookedPointing::restoreEventAction (RelativePointerEventCallback) device_:%p\n", device_);

        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(orig_relativePointerEventAction_);

        result = true;
      }
    }
    {
      ScrollWheelEventCallback callback = reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);

      if (callback == hook_ScrollWheelEventCallback) {
        IOLOG_INFO("HookedPointing::restoreEventAction (ScrollWheelEventCallback) device_:%p\n", device_);

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
