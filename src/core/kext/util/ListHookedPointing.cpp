#include "CommonData.hpp"
#include "Config.hpp"
#include "EventInputQueue.hpp"
#include "FlagStatus.hpp"
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
    IOLOG_DEBUG("ListHookedPointing::Item::~Item()\n");
    restoreEventAction();
    IOLockWrapper::free(replacerestore_lock_);
  }

  bool
  ListHookedPointing::Item::refresh_callback(void)
  {
    if (! device_) goto restore;

    // ------------------------------------------------------------
    if (! config.initialized) {
      goto restore;
    }
    if (config.get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_dont_remap_thirdvendor_pointing) &&
        deviceType_ != DeviceType::APPLE_INTERNAL &&
        deviceType_ != DeviceType::APPLE_EXTERNAL) {
      goto restore;
    }
    if (config.get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_dont_remap_apple_pointing) &&
        isEqualVendor(DeviceVendor::APPLE_COMPUTER)) {
      goto restore;
    }

    // Logitech Cordless Presenter
    if (config.get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_dont_remap_logitech_cordless_presenter) &&
        isEqualVendorProduct(DeviceVendor::LOGITECH, DeviceProduct::LOGITECH_CORDLESS_PRESENTER)) {
      goto restore;
    }
#if 0
    // Apple Magic Mouse
    if (isEqualVendorProduct(DeviceVendor::APPLE_COMPUTER, DeviceProduct::MAGIC_MOUSE)) {
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
    if (! lk) return false;

    if (! device_) return false;

    IOHIPointing* pointing = OSDynamicCast(IOHIPointing, device_);
    if (! pointing) return false;

    bool result = false;

    // ----------------------------------------
    {
      RelativePointerEventCallback callback = reinterpret_cast<RelativePointerEventCallback>(pointing->_relativePointerEventAction);

      if (callback != EventInputQueue::push_RelativePointerEventCallback) {
        IOLOG_DEBUG("ListHookedPointing::Item::replaceEventAction (RelativePointerEventCallback) device_:%p\n", device_);

        orig_relativePointerEventAction_ = callback;
        orig_relativePointerEventTarget_ = pointing->_relativePointerEventTarget;

        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(EventInputQueue::push_RelativePointerEventCallback);

        result = true;
      }
    }
    {
      ScrollWheelEventCallback callback = reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);

      if (callback != EventInputQueue::push_ScrollWheelEventCallback) {
        IOLOG_DEBUG("ListHookedPointing::Item::replaceEventAction (ScrollWheelEventCallback) device_:%p\n", device_);

        orig_scrollWheelEventAction_ = callback;
        orig_scrollWheelEventTarget_ = pointing->_scrollWheelEventTarget;

        pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(EventInputQueue::push_ScrollWheelEventCallback);

        result = true;
      }
    }

    return result;
  }

  bool
  ListHookedPointing::Item::restoreEventAction(void)
  {
    IOLockWrapper::ScopedLock lk(replacerestore_lock_);
    if (! lk) return false;

    if (! device_) return false;

    IOHIPointing* pointing = OSDynamicCast(IOHIPointing, device_);
    if (! pointing) return false;

    bool result = false;

    // ----------------------------------------
    {
      RelativePointerEventCallback callback = reinterpret_cast<RelativePointerEventCallback>(pointing->_relativePointerEventAction);

      if (callback == EventInputQueue::push_RelativePointerEventCallback) {
        IOLOG_DEBUG("HookedPointing::restoreEventAction (RelativePointerEventCallback) device_:%p\n", device_);

        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(orig_relativePointerEventAction_);

        result = true;
      }
    }
    {
      ScrollWheelEventCallback callback = reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);

      if (callback == EventInputQueue::push_ScrollWheelEventCallback) {
        IOLOG_DEBUG("HookedPointing::restoreEventAction (ScrollWheelEventCallback) device_:%p\n", device_);

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

  // ======================================================================
  void
  ListHookedPointing::Item::apply(const Params_RelativePointerEventCallback& params)
  {
    RelativePointerEventCallback callback = orig_relativePointerEventAction_;
    if (! callback) return;

    OSObject* target = orig_relativePointerEventTarget_;
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, device_);
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    params.log("sending");
    callback(target, params.buttons.get(), params.dx, params.dy, ts, sender, refcon);

    // --------------------
    if (params.buttons != Buttons(0)) {
      FlagStatus::sticky_clear();
    }
  }

  void
  ListHookedPointing::Item::apply(const Params_ScrollWheelEventCallback& params)
  {
    ScrollWheelEventCallback callback = orig_scrollWheelEventAction_;
    if (! callback) return;

    OSObject* target = orig_scrollWheelEventTarget_;
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, device_);
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    params.log("sending");
    callback(target,
             params.deltaAxis1,  params.deltaAxis2,  params.deltaAxis3,
             params.fixedDelta1, params.fixedDelta2, params.fixedDelta3,
             params.pointDelta1, params.pointDelta2, params.pointDelta3,
             params.options, ts, sender, refcon);

    // --------------------
    FlagStatus::sticky_clear();
  }

  void
  ListHookedPointing::apply(const Params_RelativePointerEventCallback& params)
  {
    IOLockWrapper::ScopedLock lk(list_lock_);
    if (! lk) return;

    // if all button are released, send event for all devices.
    if (params.buttons == Buttons(0) &&
        params.dx == 0 &&
        params.dy == 0) {
      for (Item* p = static_cast<Item*>(list_->front()); p; p = static_cast<Item*>(p->getnext())) {
        if (! p->isReplaced()) continue;

        p->apply(params);
      }

    } else {
      ListHookedPointing::Item* p = static_cast<ListHookedPointing::Item*>(get_replaced_nolock());
      if (p) {
        p->apply(params);
      }
    }
  }

  void
  ListHookedPointing::apply(const Params_ScrollWheelEventCallback& params)
  {
    IOLockWrapper::ScopedLock lk(list_lock_);
    if (! lk) return;

    ListHookedPointing::Item* p = static_cast<ListHookedPointing::Item*>(get_replaced_nolock());
    if (p) {
      p->apply(params);
    }
  }
}
