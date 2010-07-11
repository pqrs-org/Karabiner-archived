#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include "base.hpp"
#include "CallBackWrapper.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedKeyboard : public ListHookedDevice {
  public:
    class Item : public ListHookedDevice::Item {
    public:
      Item(IOHIDevice* p);
      ~Item(void);

      void apply(const Params_KeyboardEventCallBack& params);
      void apply(const Params_UpdateEventFlagsCallback& params);

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

    void apply(const Params_KeyboardEventCallBack& params);
    void apply(const Params_UpdateEventFlagsCallback& params);
  };
}

#endif
