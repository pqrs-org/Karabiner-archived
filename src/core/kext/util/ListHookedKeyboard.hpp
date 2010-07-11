#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedKeyboard : public ListHookedDevice {
  public:
    class Item : public ListHookedDevice::Item {
    public:
      Item(IOHIDevice* p);
      ~Item(void);

      IOHIKeyboard* get(void) const { return OSDynamicCast(IOHIKeyboard, ListHookedDevice::Item::get()); }

      KeyboardEventCallback getOrig_keyboardEventAction(void) const { return orig_keyboardEventAction_; }
      OSObject* getOrig_keyboardEventTarget(void) const { return orig_keyboardEventTarget_; }

      UpdateEventFlagsCallback getOrig_updateEventFlagsAction(void) const { return orig_updateEventFlagsAction_; }
      OSObject* getOrig_updateEventFlagsTarget(void) const { return orig_updateEventFlagsTarget_; }

    private:
      KeyboardEventCallback orig_keyboardEventAction_;
      OSObject* orig_keyboardEventTarget_;

      UpdateEventFlagsCallback orig_updateEventFlagsAction_;
      OSObject* orig_updateEventFlagsTarget_;

      IOLock* replacerestore_lock_;

      bool refresh_callback(void);
      bool isReplaced(void) const { return orig_keyboardEventAction_ != NULL; }

      /** return true if event action is replaced. */
      bool replaceEventAction(void);
      /** return true if event action is replaced. */
      bool restoreEventAction(void);
    };

    static ListHookedKeyboard& instance(void);
    ListHookedKeyboard::Item* get(void) { return static_cast<ListHookedKeyboard::Item*>(ListHookedDevice::get()); }
    ListHookedKeyboard::Item* get(const IOHIKeyboard* kbd) { return static_cast<ListHookedKeyboard::Item*>(ListHookedDevice::get(kbd)); }

    static void hook_KeyboardEventCallback_queued(Params_KeyboardEventCallBack& params);
  };
}

#endif
