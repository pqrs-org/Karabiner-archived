#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventWatcher.hpp"
#include "ListHookedKeyboard.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    ListHookedKeyboard listHookedKeyboard;
  }

  ListHookedKeyboard&
  ListHookedKeyboard::instance(void)
  {
    return listHookedKeyboard;
  }

  // ----------------------------------------------------------------------
  namespace {
    void
    hook_KeyboardEventCallback(OSObject* target,
                               unsigned int eventType,
                               unsigned int flags,
                               unsigned int key,
                               unsigned int charCode,
                               unsigned int charSet,
                               unsigned int origCharCode,
                               unsigned int origCharSet,
                               unsigned int keyboardType,
                               bool repeat,
                               AbsoluteTime ts,
                               OSObject* sender,
                               void* refcon)
    {
      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, sender);
      if (! kbd) return;

      HookedKeyboard* hk = ListHookedKeyboard::instance().get(kbd);
      if (! hk) return;

      // ------------------------------------------------------------
      Params_KeyboardEventCallBack params(EventType(eventType), Flags(flags), KeyCode(key),
                                          CharCode(charCode), CharSet(charSet), OrigCharCode(origCharCode), OrigCharSet(origCharSet),
                                          KeyboardType(keyboardType), repeat);
      CommonData::setcurrent_ts(ts);
      CommonData::setcurrent_keyboardType(keyboardType);

      if (params.eventType.isKeyDownOrModifierDown(params.key, params.flags) &&
          params.repeat == false) {
        EventWatcher::on();
      }

      Core::remap_KeyboardEventCallback(params);
    }
  }

  HookedKeyboard::HookedKeyboard(void) :
    isAppleDriver_(false),
    isInternalKeyboard_(false),
    orig_keyboardEventAction_(NULL), orig_keyboardEventTarget_(NULL)
  {}

  bool
  HookedKeyboard::initialize(IOHIDevice* d)
  {
    const char* name = d->getName();
    if (strcmp(name, "IOHIDConsumer") == 0) return false;
    if (HookedDevice::isIgnoreDevice(d)) return false;

    // ------------------------------------------------------------
    device_ = d;
    IOLog("KeyRemap4MacBook HookedKeyboard::initialize name = %s, device_ = 0x%p\n", name, device_);

    if (strcmp(name, "IOHIDKeyboard") == 0 ||
        strcmp(name, "AppleADBKeyboard") == 0) {
      isAppleDriver_ = true;
    } else {
      isAppleDriver_ = false;
    }

    isInternalKeyboard_ = KeyboardType(device_->deviceType()).isInternalKeyboard();

    return refresh();
  }

  bool
  HookedKeyboard::refresh(void)
  {
    if (! config.initialized) {
      return restoreEventAction();
    }
    if (! isAppleDriver_ && config.general_dont_remap_thirdvendor_keyboard) {
      return restoreEventAction();
    }
    if (isInternalKeyboard_) {
      if (config.general_dont_remap_internal) {
        return restoreEventAction();
      }
    } else {
      if (config.general_dont_remap_external) {
        return restoreEventAction();
      }
    }

    return replaceEventAction();
  }

  bool
  HookedKeyboard::terminate(void)
  {
    bool result = restoreEventAction();

    device_ = NULL;
    orig_keyboardEventAction_ = NULL;
    orig_keyboardEventTarget_ = NULL;

    return result;
  }

  bool
  HookedKeyboard::replaceEventAction(void)
  {
    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
    if (callback == hook_KeyboardEventCallback) return false;

    // ------------------------------------------------------------
    IOLog("KeyRemap4MacBook HookedKeyboard::replaceEventAction (device_ = 0x%p)\n", device_);

    orig_keyboardEventAction_ = callback;
    orig_keyboardEventTarget_ = kbd->_keyboardEventTarget;

    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(hook_KeyboardEventCallback);

    return true;
  }

  bool
  HookedKeyboard::restoreEventAction(void)
  {
    if (! device_) return false;

    IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, device_);
    if (! kbd) return false;

    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
    if (callback != hook_KeyboardEventCallback) return false;

    // ----------------------------------------
    IOLog("KeyRemap4MacBook HookedKeyboard::restoreEventAction (device_ = 0x%p)\n", device_);

    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(orig_keyboardEventAction_);

    orig_keyboardEventAction_ = NULL;
    orig_keyboardEventTarget_ = NULL;

    return true;
  }
}
