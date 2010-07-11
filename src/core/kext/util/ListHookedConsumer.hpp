#ifndef LISTHOOKEDCONSUMER_HPP
#define LISTHOOKEDCONSUMER_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedConsumer : public ListHookedDevice {
  public:
    class Item : public ListHookedDevice::Item {
    public:
      Item(IOHIDevice *p);
      ~Item(void);

      IOHIKeyboard* get(void) const { return OSDynamicCast(IOHIKeyboard, ListHookedDevice::Item::get()); }

      KeyboardSpecialEventCallback getOrig_keyboardSpecialEventAction(void) const { return orig_keyboardSpecialEventAction_; }
      OSObject* getOrig_keyboardSpecialEventTarget(void) const { return orig_keyboardSpecialEventTarget_; }

    private:
      KeyboardSpecialEventCallback orig_keyboardSpecialEventAction_;
      OSObject* orig_keyboardSpecialEventTarget_;

      IOLock* replacerestore_lock_;

      bool refresh_callback(void);
      bool isReplaced(void) const { return orig_keyboardSpecialEventAction_ != NULL; }

      /** return true if event action is replaced. */
      bool replaceEventAction(void);
      /** return true if event action is replaced. */
      bool restoreEventAction(void);
    };

    static ListHookedConsumer& instance(void);
    ListHookedConsumer::Item* get(void) { return static_cast<ListHookedConsumer::Item*>(ListHookedDevice::get()); }
    ListHookedConsumer::Item* get(const IOHIKeyboard* kbd) { return static_cast<ListHookedConsumer::Item*>(ListHookedDevice::get(kbd)); }

    static void hook_KeyboardSpecialEventCallback_queued(Params_KeyboardSpecialEventCallback& params);
  };
}

#endif
