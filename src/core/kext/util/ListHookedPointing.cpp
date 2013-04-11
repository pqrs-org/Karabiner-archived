#include "CommonData.hpp"
#include "Config.hpp"
#include "EventInputQueue.hpp"
#include "FlagStatus.hpp"
#include "GlobalLock.hpp"
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
    previousbuttons_(0)
  {}

  ListHookedPointing::Item::~Item(void)
  {
    IOLOG_DEBUG("ListHookedPointing::Item::~Item()\n");
    restoreEventAction();
  }

  bool
  ListHookedPointing::Item::refresh(void)
  {
    if (! device_) goto restore;

    // ------------------------------------------------------------
    if (! Config::get_initialized()) {
      goto restore;
    }
    if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_dont_remap_any_pointing)) {
      goto restore;
    }
    if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_dont_remap_thirdvendor_pointing) &&
        deviceType_ != DeviceType::APPLE_INTERNAL &&
        deviceType_ != DeviceType::APPLE_EXTERNAL) {
      goto restore;
    }
    if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_dont_remap_apple_pointing) &&
        getDeviceIdentifier().isEqualVendor(DeviceVendor::APPLE_COMPUTER)) {
      goto restore;
    }
    if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_dont_remap_non_apple_pointing) &&
        ! getDeviceIdentifier().isEqualVendor(DeviceVendor::APPLE_COMPUTER)) {
      goto restore;
    }

    // Logitech Cordless Presenter
    if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_dont_remap_logitech_cordless_presenter) &&
        getDeviceIdentifier().isEqualVendorProduct(DeviceVendor::LOGITECH, DeviceProduct::LOGITECH_CORDLESS_PRESENTER)) {
      goto restore;
    }

    return replaceEventAction();

  restore:
    return restoreEventAction();
  }

  bool
  ListHookedPointing::Item::replaceEventAction(void)
  {
    if (! device_) return false;

    IOHIPointing* pointing = OSDynamicCast(IOHIPointing, device_);
    if (! pointing) return false;

    bool result = false;

    // ------------------------------------------------------------
    {
      // We need to replace _relativePointerEventAction until it points KeyRemap4MacBook's callback function.
      // (A reason is described at ListHookedKeyboard::replaceEventAction.)
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
      // We need to replace _scrollWheelEventAction until it points KeyRemap4MacBook's callback function.
      // (A reason is described at ListHookedKeyboard::replaceEventAction.)
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

    Params_RelativePointerEventCallback::log(false, params.buttons, params.dx, params.dy);
    {
      // We need to unlock the global lock while we are calling the callback function.
      // For more information, See ListHookedKeyboard::Item::apply(const Params_KeyboardEventCallBack& params)
      GlobalLock::ScopedUnlock lk;
      callback(target, params.buttons.get(), params.dx, params.dy, ts, sender, refcon);
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

    Params_ScrollWheelEventCallback::log(false,
                                         params.deltaAxis1,  params.deltaAxis2,  params.deltaAxis3,
                                         params.fixedDelta1, params.fixedDelta2, params.fixedDelta3,
                                         params.pointDelta1, params.pointDelta2, params.pointDelta3,
                                         params.options);
    {
      // We need to unlock the global lock while we are calling the callback function.
      // For more information, See ListHookedKeyboard::Item::apply(const Params_KeyboardEventCallBack& params)
      GlobalLock::ScopedUnlock lk;
      callback(target,
               params.deltaAxis1,  params.deltaAxis2,  params.deltaAxis3,
               params.fixedDelta1, params.fixedDelta2, params.fixedDelta3,
               params.pointDelta1, params.pointDelta2, params.pointDelta3,
               params.options, ts, sender, refcon);
    }
  }

  void
  ListHookedPointing::apply(const Params_RelativePointerEventCallback& params)
  {
    // if all button are released, send event for all devices.
    if (params.buttons == Buttons(0) &&
        params.dx == 0 &&
        params.dy == 0) {
      for (Item* p = static_cast<Item*>(list_->front()); p; p = static_cast<Item*>(p->getnext())) {
        if (! p->isReplaced()) continue;

        p->apply(params);
      }

    } else {
      ListHookedPointing::Item* p = static_cast<ListHookedPointing::Item*>(get_replaced());
      if (p) {
        p->apply(params);
      }
    }
  }

  void
  ListHookedPointing::apply(const Params_ScrollWheelEventCallback& params)
  {
    ListHookedPointing::Item* p = static_cast<ListHookedPointing::Item*>(get_replaced());
    if (p) {
      p->apply(params);
    }
  }
}
