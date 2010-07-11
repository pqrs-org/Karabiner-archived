#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventInputQueue.hpp"
#include "EventWatcher.hpp"
#include "FlagStatus.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedConsumer.hpp"
#include "NumHeldDownKeys.hpp"

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
    IOLOG_INFO("ListHookedConsumer::Item::~Item()\n");
    IOLockWrapper::free(replacerestore_lock_);
    restoreEventAction();
  }

  // ----------------------------------------------------------------------
  void
  ListHookedConsumer::hook_KeyboardSpecialEventCallback(OSObject* target,
                                                        unsigned int eventType,
                                                        unsigned int flags,
                                                        unsigned int key,
                                                        unsigned int flavor,
                                                        UInt64 guid,
                                                        bool repeat,
                                                        AbsoluteTime ts,
                                                        OSObject* sender,
                                                        void* refcon)
  {
    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(EventType(eventType), Flags(flags), ConsumerKeyCode(key),
                                                                                                 flavor, guid, repeat));
    if (! ptr) return;
    Params_KeyboardSpecialEventCallback& params = *ptr;

    {
      if (! CommonData::eventLock) return;
      IOLockWrapper::ScopedLock lk(CommonData::eventLock);

      IOLockWrapper::ScopedLock lk2(ListHookedConsumer::instance().list_lock_);

      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, sender);
      if (! kbd) return;

      ListHookedConsumer::Item* hc = static_cast<ListHookedConsumer::Item*>(ListHookedConsumer::instance().get_nolock(kbd));
      if (! hc) return;

      // ------------------------------------------------------------
      CommonData::setcurrent_ts(ts);
      CommonData::setcurrent_vendorIDproductID(hc->getVendorID(), hc->getProductID());

      // ------------------------------------------------------------
      // Because we handle the key repeat ourself, drop the key repeat by hardware.
      if (repeat) return;

      // ------------------------------------------------------------
      if (params.eventType == EventType::DOWN) {
        CommonData::setcurrent_workspacedata();
      }

      // ------------------------------------------------------------
      // clear temporary_count_
      FlagStatus::set();
    }

    // ------------------------------------------------------------
    EventInputQueue::push(params);
  }

  void
  ListHookedConsumer::hook_KeyboardSpecialEventCallback_queued(Params_KeyboardSpecialEventCallback& params)
  {
    if (! CommonData::eventLock) return;
    IOLockWrapper::ScopedLock lk(CommonData::eventLock);

    if (params.eventType == EventType::DOWN) {
      EventWatcher::on();
    }

    // ------------------------------------------------------------
    NumHeldDownKeys::set(params.eventType);

    Core::remap_KeyboardSpecialEventCallback(params);
  }

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
    if (callback == hook_KeyboardSpecialEventCallback) return false;

    // ------------------------------------------------------------
    IOLOG_INFO("HookedConsumer::replaceEventAction device_:%p\n", device_);

    orig_keyboardSpecialEventAction_ = callback;
    orig_keyboardSpecialEventTarget_ = kbd->_keyboardSpecialEventTarget;

    kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(hook_KeyboardSpecialEventCallback);

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
    if (callback != hook_KeyboardSpecialEventCallback) return false;

    // ----------------------------------------
    IOLOG_INFO("HookedConsumer::restoreEventAction device_:%p\n", device_);

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
    if (params.eventType == EventType::DOWN) {
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

    ListHookedConsumer::Item* p = static_cast<ListHookedConsumer::Item*>(get_nolock());
    if (p) {
      p->apply(params);
    }
  }

  void
  ListHookedConsumer::disableNumLock(void)
  {
    IOLockWrapper::ScopedLock lk(list_lock_);

    ListHookedConsumer::Item* p = static_cast<ListHookedConsumer::Item*>(get_nolock());
    if (p) {
      p->disableNumLock();
    }
  }
}
