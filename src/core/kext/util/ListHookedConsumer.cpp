#include "ListHookedConsumer.hpp"
#include "Core.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  ListHookedConsumer listHookedConsumer;

  ListHookedConsumer &
  ListHookedConsumer::instance(void)
  {
    return listHookedConsumer;
  }

  // ----------------------------------------------------------------------
  namespace {
    void
    hook_KeyboardSpecialEventCallback(OSObject *target,
                                      unsigned int eventType,
                                      unsigned int flags,
                                      unsigned int key,
                                      unsigned int flavor,
                                      UInt64 guid,
                                      bool repeat,
                                      AbsoluteTime ts,
                                      OSObject *sender,
                                      void *refcon)
    {
      Params_KeyboardSpecialEventCallback params = {
        target, eventType, flags, key, flavor,
        guid, repeat, ts, sender, refcon,
      };
      Core::remap_KeyboardSpecialEventCallback(&params);
    }
  }

  HookedConsumer::HookedConsumer(void) :
    orig_keyboardSpecialEventAction(NULL), orig_keyboardSpecialEventTarget(NULL)
  {
  }

  bool
  HookedConsumer::initialize(IOHIDevice *_device)
  {
    const char *name = _device->getName();
    if (strcmp(name, "IOHIDConsumer") != 0) return false;

    device = _device;
    IOLog("KeyRemap4MacBook HookedConsumer::initialize name = %s, device = 0x%p\n", name, device);

    return refresh();
  }

  bool
  HookedConsumer::refresh(void)
  {
    if (! device) return false;

    IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, device);
    if (! kbd) return false;

    // ----------------------------------------
    KeyboardSpecialEventCallback callback = reinterpret_cast<KeyboardSpecialEventCallback>(kbd->_keyboardSpecialEventAction);

    if (callback != hook_KeyboardSpecialEventCallback) {
      IOLog("KeyRemap4MacBook [refresh] HookedConsumer::refresh (device = 0x%p)\n", device);

      orig_keyboardSpecialEventAction = callback;
      orig_keyboardSpecialEventTarget = kbd->_keyboardSpecialEventTarget;

      kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(hook_KeyboardSpecialEventCallback);
    }

    return true;
  }

  bool
  HookedConsumer::terminate(void)
  {
    if (! device) return false;

    IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, device);
    if (! kbd) return false;

    // ----------------------------------------
    KeyboardSpecialEventCallback callback = reinterpret_cast<KeyboardSpecialEventCallback>(kbd->_keyboardSpecialEventAction);
    if (callback == hook_KeyboardSpecialEventCallback) {
      kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(orig_keyboardSpecialEventAction);
    }

    device = NULL;
    return true;
  }
}
