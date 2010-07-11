#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventInputQueue.hpp"
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

  ListHookedPointing::Item::Item(IOHIDevice* p) : ListHookedDevice::Item(p),
                                                  orig_relativePointerEventAction_(NULL),
                                                  orig_relativePointerEventTarget_(NULL),
                                                  orig_scrollWheelEventAction_(NULL),
                                                  orig_scrollWheelEventTarget_(NULL),
                                                  previousbuttons_(0),
                                                  replacerestore_lock_(NULL)
  {
    replacerestore_lock_ = IOLockWrapper::alloc();
  }

  ListHookedPointing::Item::~Item(void)
  {
    IOLOG_INFO("ListHookedPointing::Item::~Item()\n");
    IOLockWrapper::free(replacerestore_lock_);
    restoreEventAction();
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
      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(buttons, dx, dy));
      if (! ptr) return;
      Params_RelativePointerEventCallback& params = *ptr;

      Buttons justPressed;
      Buttons justReleased;

      {
        if (! CommonData::eventLock) return;
        IOLockWrapper::ScopedLock lk(CommonData::eventLock);

        IOHIPointing* pointing = OSDynamicCast(IOHIPointing, sender);
        if (! pointing) return;

        ListHookedPointing::Item* hp = ListHookedPointing::instance().get(pointing);
        if (! hp) return;

        // ------------------------------------------------------------
        CommonData::setcurrent_ts(ts);
        CommonData::setcurrent_vendorIDproductID(hp->getVendorID(), hp->getProductID());

        // ------------------------------------------------------------
        justPressed = params.buttons.justPressed(hp->get_previousbuttons());
        justReleased = params.buttons.justReleased(hp->get_previousbuttons());
        hp->set_previousbuttons(buttons);

        if (justPressed != Buttons(0)) {
          CommonData::setcurrent_workspacedata();
        }

        // ------------------------------------------------------------
        // clear temporary_count_
        if (! config.general_lazy_modifiers_with_mouse_event) {
          FlagStatus::set();
        }
      }

      // ------------------------------------------------------------
      // divide an event into button and cursormove events.
      params.dx = 0;
      params.dy = 0;
      params.ex_button = PointingButton::NONE;
      params.ex_isbuttondown = false;

      for (int i = 0; i < ButtonStatus::MAXNUM; ++i) {
        PointingButton btn(1 << i);
        if (justPressed.isOn(btn)) {
          params.ex_button = btn;
          params.ex_isbuttondown = true;
          EventInputQueue::push(params);
        }
        if (justReleased.isOn(btn)) {
          params.ex_button = btn;
          params.ex_isbuttondown = false;
          EventInputQueue::push(params);
        }
      }
      if (dx != 0 || dy != 0) {
        params.dx = dx;
        params.dy = dy;
        params.ex_button = PointingButton::NONE;
        EventInputQueue::push(params);
      }
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
      Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(deltaAxis1, deltaAxis2, deltaAxis3,
                                                                                           fixedDelta1, fixedDelta2, fixedDelta3,
                                                                                           pointDelta1, pointDelta2, pointDelta3,
                                                                                           options));
      if (! ptr) return;
      Params_ScrollWheelEventCallback& params = *ptr;

      {
        if (! CommonData::eventLock) return;
        IOLockWrapper::ScopedLock lk(CommonData::eventLock);

        IOHIPointing* pointing = OSDynamicCast(IOHIPointing, sender);
        if (! pointing) return;

        ListHookedPointing::Item* hp = ListHookedPointing::instance().get(pointing);
        if (! hp) return;

        // ------------------------------------------------------------
        CommonData::setcurrent_ts(ts);
        CommonData::setcurrent_vendorIDproductID(hp->getVendorID(), hp->getProductID());

        // ------------------------------------------------------------
        // clear temporary_count_
        if (! config.general_lazy_modifiers_with_mouse_event) {
          FlagStatus::set();
        }
      }

      // ------------------------------------------------------------
      EventInputQueue::push(params);
    }
  }

  void
  ListHookedPointing::hook_RelativePointerEventCallback_queued(Params_RelativePointerEventCallback& params)
  {
    if (! CommonData::eventLock) return;
    IOLockWrapper::ScopedLock lk(CommonData::eventLock);

    // ------------------------------------------------------------
    // We set EventWatcher::on only when Buttons pressed.
    // It's cause a problem when you use the following settings. (Unexpected FN_Lock is fired).
    //   - FN+CursorMove to ScrollWheel
    //   - FN to FN (+ When you type FN only, send FN_Lock)
    //
    // But, if we call EventWatcher::on every CursorMove event, unexpected cancel occurs.
    // It's more terrible than above problem.
    // So, we keep to call EventWatcher::on only when Buttons pressed.
    if (params.ex_button != PointingButton::NONE) {
      EventWatcher::on();
    }

    // ------------------------------------------------------------
    if (params.ex_button != PointingButton::NONE) {
      if (params.ex_isbuttondown) {
        NumHeldDownKeys::set(1);
      } else {
        NumHeldDownKeys::set(-1);
      }
    }

    Core::remap_RelativePointerEventCallback(params);
  }

  void
  ListHookedPointing::hook_ScrollWheelEventCallback_queued(Params_ScrollWheelEventCallback& params)
  {
    if (! CommonData::eventLock) return;
    IOLockWrapper::ScopedLock lk(CommonData::eventLock);

    // EventWatcher::on is not necessary.

    Core::remap_ScrollWheelEventCallback(params);
  }

  bool
  ListHookedPointing::Item::refresh_callback(void)
  {
    if (! device_) goto restore;

    // ------------------------------------------------------------
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
  ListHookedPointing::Item::replaceEventAction(void)
  {
    IOLockWrapper::ScopedLock lk(replacerestore_lock_);

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
  ListHookedPointing::Item::restoreEventAction(void)
  {
    IOLockWrapper::ScopedLock lk(replacerestore_lock_);

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
