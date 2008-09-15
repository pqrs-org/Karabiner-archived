#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace ListHookedKeyboard {
    void terminate(void);

    class Item;
    bool append(IOHIKeyboard *kbd);
    Item *get(const IOHIKeyboard *kbd);
    Item *get(void);
    void refresh(void);

    // ----------------------------------------------------------------------
    class Item {
    public:
      Item(void) { kbd = NULL; }

      void initialize(IOHIKeyboard *_kbd);
      void refresh(void);
      void terminate(void);

      IOHIKeyboard *get(void) const { return kbd; }

    private:
      IOHIKeyboard *kbd;
      KeyboardEventCallback orig_keyboardEventAction;
      KeyboardSpecialEventCallback orig_keyboardSpecialEventAction;
      OSObject *orig_keyboardEventTarget;
      OSObject *orig_keyboardSpecialEventTarget;

      KeyboardEventCallback getCurrent_keyboardEventAction(void) const {
        if (kbd == NULL) return NULL;
        return reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
      }
      KeyboardSpecialEventCallback getCurrent_keyboardSpecialEventAction(void) const {
        if (kbd == NULL) return NULL;
        return reinterpret_cast<KeyboardSpecialEventCallback>(kbd->_keyboardSpecialEventAction);
      }
    };

    // ----------------------------------------------------------------------
    void hook_KeyboardEventCallback(OSObject *target,
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
                                    void *refcon);
    void hook_KeyboardSpecialEventCallback(OSObject *target,
                                           unsigned int eventType,
                                           unsigned int flags,
                                           unsigned int key,
                                           unsigned int flavor,
                                           UInt64 guid,
                                           bool repeat,
                                           AbsoluteTime ts,
                                           OSObject *sender,
                                           void *refcon);
  }
}

#endif
