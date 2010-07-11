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

    static void hook_KeyboardEventCallback_queued(Params_KeyboardEventCallBack& params);

    void apply(const Params_KeyboardEventCallBack& params);
    void apply(const Params_UpdateEventFlagsCallback& params);

  private:
    static void hook_KeyboardEventCallback(OSObject* target,
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
                                           OSObject* sender,
                                           void* refcon);

    static void hook_UpdateEventFlagsCallback(OSObject* target,
                                              unsigned flags,
                                              OSObject* sender,
                                              void* refcon);
  };
}

#endif
