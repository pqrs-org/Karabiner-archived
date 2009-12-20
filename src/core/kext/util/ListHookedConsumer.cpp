#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventWatcher.hpp"
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

  // ----------------------------------------------------------------------
  namespace {
    void
    hook_KeyboardSpecialEventCallback(OSObject* target,
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
      if (! CommonData::eventLock) return;
      IOLockWrapper::ScopedLock lk(CommonData::eventLock);

      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, sender);
      if (! kbd) return;

      HookedConsumer* hc = ListHookedConsumer::instance().get(kbd);
      if (! hc) return;

      // ------------------------------------------------------------
      CommonData::setcurrent_ts(ts);

      // ------------------------------------------------------------
      Params_KeyboardSpecialEventCallback params(EventType(eventType), Flags(flags), ConsumerKeyCode(key),
                                                 flavor, guid, repeat);
      if (params.eventType == EventType::DOWN &&
          params.repeat == false) {
        EventWatcher::on();
      }

      Core::remap_KeyboardSpecialEventCallback(params);
    }
  }

  HookedConsumer::HookedConsumer(void) :
    orig_keyboardSpecialEventAction_(NULL), orig_keyboardSpecialEventTarget_(NULL)
  {}

  bool
  HookedConsumer::initialize(IOHIDevice* d)
  {
    if (! d) return false;

    const char* name = d->getName();
    if (! name) return false;

    if (strcmp(name, "IOHIDConsumer") != 0) return false;
    if (HookedDevice::isIgnoreDevice(d)) return false;

    device_ = d;
    IOLog("KeyRemap4MacBook HookedConsumer::initialize name = %s, device = 0x%p\n", name, device_);

    return refresh();
  }

  bool
  HookedConsumer::refresh(void)
  {
    if (! config.initialized) {
      return restoreEventAction();
    }
    return replaceEventAction();
  }

  bool
  HookedConsumer::terminate(void)
  {
    bool result = restoreEventAction();

    device_ = NULL;
    orig_keyboardSpecialEventAction_ = NULL;
    orig_keyboardSpecialEventTarget_ = NULL;

    return result;
  }

  bool
  HookedConsumer::replaceEventAction(void)
  {
    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardSpecialEventCallback callback = reinterpret_cast<KeyboardSpecialEventCallback>(kbd->_keyboardSpecialEventAction);
    if (callback == hook_KeyboardSpecialEventCallback) return false;

    // ------------------------------------------------------------
    IOLog("KeyRemap4MacBook HookedConsumer::replaceEventAction (device_ = 0x%p)\n", device_);

    orig_keyboardSpecialEventAction_ = callback;
    orig_keyboardSpecialEventTarget_ = kbd->_keyboardSpecialEventTarget;

    kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(hook_KeyboardSpecialEventCallback);

    return true;
  }

  bool
  HookedConsumer::restoreEventAction(void)
  {
    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardSpecialEventCallback callback = reinterpret_cast<KeyboardSpecialEventCallback>(kbd->_keyboardSpecialEventAction);
    if (callback != hook_KeyboardSpecialEventCallback) return false;

    // ----------------------------------------
    IOLog("KeyRemap4MacBook HookedConsumer::restoreEventAction (device_ = 0x%p)\n", device_);

    kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(orig_keyboardSpecialEventAction_);

    orig_keyboardSpecialEventAction_ = NULL;
    orig_keyboardSpecialEventTarget_ = NULL;

    return true;
  }
}
