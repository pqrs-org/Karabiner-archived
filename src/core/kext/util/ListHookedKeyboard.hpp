#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedKeyboard : public HookedDevice {
  public:
    HookedKeyboard(void);

    IOHIKeyboard* get(void) const { return OSDynamicCast(IOHIKeyboard, HookedDevice::get()); }

    KeyboardEventCallback getOrig_keyboardEventAction(void) const { return orig_keyboardEventAction_; }
    OSObject* getOrig_keyboardEventTarget(void) const { return orig_keyboardEventTarget_; }

  private:
    bool isAppleDriver_;
    bool isInternalKeyboard_;

    KeyboardEventCallback orig_keyboardEventAction_;
    OSObject* orig_keyboardEventTarget_;

    bool initialize(IOHIDevice* d);
    bool refresh(void);
    bool terminate(void);

    /** return true if event action is replaced. */
    bool replaceEventAction(void);
    /** return true if event action is replaced. */
    bool restoreEventAction(void);
  };

  class ListHookedKeyboard : public ListHookedDevice {
  public:
    static ListHookedKeyboard& instance(void);
    HookedKeyboard* get(void) { return static_cast<HookedKeyboard*>(ListHookedDevice::get()); }
    HookedKeyboard* get(const IOHIKeyboard* kbd) { return static_cast<HookedKeyboard*>(ListHookedDevice::get(kbd)); }

  private:
    HookedKeyboard item_[MAXNUM];

    HookedDevice* getItem(int index) {
      if (index < 0 || index >= MAXNUM) return NULL;
      return item_ + index;
    }
  };
}

#endif
