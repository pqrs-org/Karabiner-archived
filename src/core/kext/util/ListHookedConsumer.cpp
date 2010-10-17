#include "CommonData.hpp"
#include "Config.hpp"
#include "EventInputQueue.hpp"
#include "FlagStatus.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedConsumer.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    ListHookedConsumer listHookedConsumer;
  }

  ListHookedConsumer&
  ListHookedConsumer::instance(void)
  {
    return listHookedConsumer;
  }

  ListHookedConsumer::Item::Item(IOHIDevice* p) : ListHookedDevice::Item(p),
    orig_keyboardSpecialEventAction_(NULL),
    orig_keyboardSpecialEventTarget_(NULL),
    replacerestore_lock_(NULL)
  {
    replacerestore_lock_ = IOLockWrapper::alloc();
  }

  ListHookedConsumer::Item::~Item(void)
  {
    IOLOG_DEBUG("ListHookedConsumer::Item::~Item()\n");
    IOLockWrapper::free(replacerestore_lock_);
    restoreEventAction();
  }

  // ----------------------------------------------------------------------
  bool
  ListHookedConsumer::Item::refresh_callback(void)
  {
    if (! device_) goto restore;

    {
      const char* name = device_->getName();
      if (! name) goto restore;

      if (! ListHookedDevice::Item::isConsumer(name)) goto restore;
    }

    // ------------------------------------------------------------
    if (! config.initialized) {
      goto restore;
    }
    if (config.general_dont_remap_apple_keyboard &&
        isEqualVendorID(DeviceVendorID(0x05ac))) {
      goto restore;
    }
    // Logitech USB Headset
    if (isEqualVendorIDProductID(DeviceVendorID(0x046d), DeviceProductID(0x0a0b))) {
      goto restore;
    }
    // Logitech Cordless Presenter
    if (config.general_dont_remap_logitech_cordless_presenter &&
        isEqualVendorIDProductID(DeviceVendorID(0x046d), DeviceProductID(0xc515))) {
      goto restore;
    }
#if 0
    // Apple Internal Keyboard
    if (isEqualVendorIDProductID(DeviceVendorID(0x05ac), DeviceProductID(0x21a))) {
      goto restore;
    }
#endif
#if 0
    // Apple External Keyboard
    if (isEqualVendorIDProductID(DeviceProductID(0x05ac), DeviceProductID(0x0222))) {
      goto restore;
    }
#endif

    return replaceEventAction();

  restore:
    return restoreEventAction();
  }

  bool
  ListHookedConsumer::Item::replaceEventAction(void)
  {
    IOLockWrapper::ScopedLock lk(replacerestore_lock_);

    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardSpecialEventCallback callback = reinterpret_cast<KeyboardSpecialEventCallback>(kbd->_keyboardSpecialEventAction);
    if (callback == EventInputQueue::push_KeyboardSpecialEventCallback) return false;

    // ------------------------------------------------------------
    IOLOG_DEBUG("HookedConsumer::replaceEventAction device_:%p\n", device_);

    orig_keyboardSpecialEventAction_ = callback;
    orig_keyboardSpecialEventTarget_ = kbd->_keyboardSpecialEventTarget;

    kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(EventInputQueue::push_KeyboardSpecialEventCallback);

    return true;
  }

  bool
  ListHookedConsumer::Item::restoreEventAction(void)
  {
    IOLockWrapper::ScopedLock lk(replacerestore_lock_);

    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardSpecialEventCallback callback = reinterpret_cast<KeyboardSpecialEventCallback>(kbd->_keyboardSpecialEventAction);
    if (callback != EventInputQueue::push_KeyboardSpecialEventCallback) return false;

    // ----------------------------------------
    IOLOG_DEBUG("HookedConsumer::restoreEventAction device_:%p\n", device_);

    kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(orig_keyboardSpecialEventAction_);

    orig_keyboardSpecialEventAction_ = NULL;
    orig_keyboardSpecialEventTarget_ = NULL;

    return true;
  }

  // ======================================================================
  void
  ListHookedConsumer::Item::apply(const Params_KeyboardSpecialEventCallback& params)
  {
    if (params.key >= ConsumerKeyCode::VK__BEGIN__) {
      // Invalid keycode
      IOLOG_ERROR("%s invalid key:%d\n", __PRETTY_FUNCTION__, params.key.get());
      return;
    }
    if (params.flags.isVirtualModifiersOn()) {
      IOLOG_ERROR("%s invalid flags:%d\n", __PRETTY_FUNCTION__, params.flags.get());
      return;
    }

    // ------------------------------------------------------------
    KeyboardSpecialEventCallback callback = orig_keyboardSpecialEventAction_;
    if (! callback) return;

    OSObject* target = orig_keyboardSpecialEventTarget_;
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, device_);
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    params.log("sending");
    callback(target, params.eventType.get(), params.flags.get(), params.key.get(),
             params.flavor, params.guid, params.repeat, ts, sender, refcon);

    // --------------------
    if (params.ex_iskeydown) {
      FlagStatus::sticky_clear();
    }
  }

  void
  ListHookedConsumer::Item::disableNumLock(void)
  {
    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return;

    if (kbd->numLock()) {
      kbd->setNumLock(false);
    }
  }

  void
  ListHookedConsumer::apply(const Params_KeyboardSpecialEventCallback& params)
  {
    IOLockWrapper::ScopedLock lk(list_lock_);

    ListHookedConsumer::Item* p = static_cast<ListHookedConsumer::Item*>(get_replaced_nolock());
    if (p) {
      p->apply(params);
    }
  }

  void
  ListHookedConsumer::disableNumLock(void)
  {
    IOLockWrapper::ScopedLock lk(list_lock_);

    ListHookedConsumer::Item* p = static_cast<ListHookedConsumer::Item*>(get_replaced_nolock());
    if (p) {
      p->disableNumLock();
    }
  }
}
