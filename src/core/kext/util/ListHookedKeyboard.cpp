#include "EventWatcher.hpp"
#include "ListHookedKeyboard.hpp"
#include "Core.hpp"
#include "Config.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    ListHookedKeyboard listHookedKeyboard;
  }

  ListHookedKeyboard &
  ListHookedKeyboard::instance(void)
  {
    return listHookedKeyboard;
  }

  // ----------------------------------------------------------------------
  namespace {
    void
    hook_KeyboardEventCallback(OSObject *target,
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
                               OSObject *sender,
                               void *refcon)
    {
      EventWatcher::countup();

      Params_KeyboardEventCallBack params = {
        target, eventType, flags, key,
        charCode, charSet, origCharCode, origCharSet,
        keyboardType, repeat, ts, sender, refcon,
      };
      Core::remap_KeyboardEventCallback(params);
    }
  }

  HookedKeyboard::HookedKeyboard(void) :
    isAppleDriver(false),
    isInternalKeyboard(false),
    orig_keyboardEventAction(NULL), orig_keyboardEventTarget(NULL)
  {
  }

  bool
  HookedKeyboard::initialize(IOHIDevice *_device)
  {
    const char *name = _device->getName();
    if (strcmp(name, "IOHIDConsumer") == 0) return false;
    if (HookedDevice::isIgnoreDevice(_device)) return false;

    // ------------------------------------------------------------
    device = _device;
    IOLog("KeyRemap4MacBook HookedKeyboard::initialize name = %s, device = 0x%p\n", name, device);

    if (strcmp(name, "IOHIDKeyboard") == 0 ||
        strcmp(name, "AppleADBKeyboard") == 0) {
      isAppleDriver = true;
    } else {
      isAppleDriver = false;
    }

    isInternalKeyboard = RemapUtil::isInternalKeyboard(device->deviceType());

    return refresh();
  }

  bool
  HookedKeyboard::refresh(void)
  {
    if (! config.initialized) {
      return restoreEventAction();
    }
    if (! isAppleDriver && config.general_dont_remap_thirdvendor_keyboard) {
      return restoreEventAction();
    }
    if (isInternalKeyboard) {
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

    device = NULL;
    orig_keyboardEventAction = NULL;
    orig_keyboardEventTarget = NULL;

    return result;
  }

  bool
  HookedKeyboard::replaceEventAction(void)
  {
    if (! device) return false;

    IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, device);
    if (! kbd) return false;

    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
    if (callback == hook_KeyboardEventCallback) return false;

    // ------------------------------------------------------------
    IOLog("KeyRemap4MacBook HookedKeyboard::replaceEventAction (device = 0x%p)\n", device);

    orig_keyboardEventAction = callback;
    orig_keyboardEventTarget = kbd->_keyboardEventTarget;

    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(hook_KeyboardEventCallback);

    return true;
  }

  bool
  HookedKeyboard::restoreEventAction(void)
  {
    if (! device) return false;

    IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, device);
    if (! kbd) return false;

    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
    if (callback != hook_KeyboardEventCallback) return false;

    // ----------------------------------------
    IOLog("KeyRemap4MacBook HookedKeyboard::restoreEventAction (device = 0x%p)\n", device);

    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(orig_keyboardEventAction);

    orig_keyboardEventAction = NULL;
    orig_keyboardEventTarget = NULL;

    return true;
  }
}
