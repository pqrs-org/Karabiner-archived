#include "ListHookedKeyboard.hpp"
#include "Core.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  ListHookedKeyboard listHookedKeyboard;

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
      Params_KeyboardEventCallBack params = {
        target, eventType, flags, key,
        charCode, charSet, origCharCode, origCharSet,
        keyboardType, repeat, ts, sender, refcon,
      };
      Core::remap_KeyboardEventCallback(&params);
    }
  }

  HookedKeyboard::HookedKeyboard(void) :
    isAppleDriver(false),
    orig_keyboardEventAction(NULL), orig_keyboardEventTarget(NULL)
  {
  }

  bool
  HookedKeyboard::initialize(IOHIDevice *_device)
  {
    const char *name = _device->getName();
    if (strcmp(name, "IOHIDConsumer") == 0) return false;

    device = _device;
    IOLog("KeyRemap4MacBook HookedKeyboard::initialize name = %s, device = 0x%p\n", name, device);

    if (strcmp(name, "IOHIDKeyboard") == 0 ||
        strcmp(name, "AppleADBKeyboard") == 0) {
      isAppleDriver = true;
    } else {
      isAppleDriver = false;
    }

    return refresh();
  }

  bool
  HookedKeyboard::refresh(void)
  {
    if (! device) return false;
    if (! isAppleDriver && ! config.general_remap_thirdvendor_keyboard) return false;

    IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, device);
    if (! kbd) return false;

    // ------------------------------------------------------------
    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);

    if (callback != hook_KeyboardEventCallback) {
      IOLog("KeyRemap4MacBook [refresh] HookedKeyboard::refresh KeyboardEventCallback (device = 0x%p)\n", device);

      orig_keyboardEventAction = callback;
      orig_keyboardEventTarget = kbd->_keyboardEventTarget;

      kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(hook_KeyboardEventCallback);
    }

    return true;
  }

  bool
  HookedKeyboard::terminate(void)
  {
    if (! device) return false;

    IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, device);
    if (! kbd) return false;

    // ------------------------------------------------------------
    KeyboardEventCallback callback = reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
    if (callback == hook_KeyboardEventCallback) {
        kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(orig_keyboardEventAction);
    }

    device = NULL;
    return true;
  }
}
