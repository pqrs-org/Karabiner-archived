#include "ListHookedKeyboard.hpp"
#include "Core.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace ListHookedKeyboard {
    enum {
      MAXNUM = 16,
    };
    Item item[MAXNUM];

    bool
    append(IOHIKeyboard *kbd)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i].get()) continue;

        return item[i].initialize(kbd);
      }
      return false;
    }

    void
    terminate(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        item[i].terminate();
      }
    }

    bool
    terminate(const IOHIKeyboard *kbd)
    {
      Item *p = get(kbd);
      if (p == NULL) return false;
      p->terminate();
      return true;
    }

    Item *
    get(const IOHIKeyboard *kbd)
    {
      if (kbd == NULL) return NULL;

      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i].get() == kbd) return item + i;
      }
      return NULL;
    }

    Item *
    get(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i].get()) return item + i;
      }
      return NULL;
    }

    Item *
    getConsumer(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i].get() && item[i].isConsumer()) return item + i;
      }
      return NULL;
    }

    void
    refresh(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        item[i].refresh();
      }
    }

    // ======================================================================
    bool
    Item::initialize(IOHIKeyboard *_kbd)
    {
      const char *name = _kbd->getName();
      if (strcmp(name, "IOHIDKeyboard") != 0 &&
          strcmp(name, "AppleADBKeyboard") != 0 &&
          strcmp(name, "IOHIDConsumer") != 0) {
        IOLog("KeyRemap4MacBook replaceKeyboardEvent ignore device [%s]\n", name);
        return false;
      }

      IOLog("KeyRemap4MacBook replaceKeyboardEvent name = %s\n", name);
      kbd = _kbd;
      consumerFlag = (strcmp(name, "IOHIDConsumer") == 0);
      refresh();
      return true;
    }

    void
    Item::refresh(void)
    {
      if (kbd == NULL) return;

      {
        KeyboardEventCallback old = getCurrent_keyboardEventAction();

        if (old != hook_KeyboardEventCallback) {
          IOLog("KeyRemap4MacBook [refresh] KeyboardEventAction\n");

          orig_keyboardEventAction = old;
          kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(hook_KeyboardEventCallback);

          orig_keyboardEventTarget = kbd->_keyboardEventTarget;
        }
      }
      {
        KeyboardSpecialEventCallback old = getCurrent_keyboardSpecialEventAction();

        if (old != hook_KeyboardSpecialEventCallback) {
          IOLog("KeyRemap4MacBook [refresh] KeyboardSpecialEventAction\n");

          orig_keyboardSpecialEventAction = old;
          kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(hook_KeyboardSpecialEventCallback);

          orig_keyboardSpecialEventTarget = kbd->_keyboardSpecialEventTarget;
        }
      }
    }

    void
    Item::terminate(void)
    {
      if (! kbd) return;

      if (getCurrent_keyboardEventAction() == hook_KeyboardEventCallback) {
        kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(orig_keyboardEventAction);
      }
      if (getCurrent_keyboardSpecialEventAction() == hook_KeyboardSpecialEventCallback) {
        kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(orig_keyboardSpecialEventAction);
      }
      kbd = NULL;
    }

    // ----------------------------------------------------------------------
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
}
