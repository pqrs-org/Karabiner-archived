#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedKeyboard : public HookedDevice {
  public:
    HookedKeyboard(void);

    IOHIKeyboard *get(void) const { return OSDynamicCast(IOHIKeyboard, HookedDevice::get()); }

    KeyboardEventCallback getOrig_keyboardEventAction() const { return orig_keyboardEventAction; }
    OSObject *getOrig_keyboardEventTarget() const { return orig_keyboardEventTarget; }

  private:
    bool isAppleDriver;

    KeyboardEventCallback orig_keyboardEventAction;
    OSObject *orig_keyboardEventTarget;

    bool initialize(IOHIDevice *_device);
    bool refresh(void);
    bool terminate(void);

    bool replaceEventAction(void);
    bool restoreEventAction(void);
  };

  class ListHookedKeyboard : public ListHookedDevice {
  public:
    static ListHookedKeyboard &instance(void);
    HookedKeyboard *get(void) { return static_cast<HookedKeyboard *>(ListHookedDevice::get()); }
    HookedKeyboard *get(const IOHIKeyboard *kbd) { return static_cast<HookedKeyboard *>(ListHookedDevice::get(kbd)); }

  private:
    HookedKeyboard item[MAXNUM];

    HookedDevice *getItem(int index) {
      if (index < 0 || index >= MAXNUM) return NULL;
      return item + index;
    }
  };
}

#endif
