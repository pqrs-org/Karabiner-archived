#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include "base.hpp"
#include "CallBackWrapper.hpp"
#include "ListHookedDevice.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedKeyboard : public ListHookedDevice {
  public:
    class Item : public ListHookedDevice::Item {
    public:
      Item(IOHIDevice* p);
      ~Item(void);

      bool isReplaced(void) const { return orig_keyboardEventAction_ != NULL; }

      void apply(const Params_KeyboardEventCallBack& params);
      void apply(const Params_UpdateEventFlagsCallback& params);

    private:
      KeyboardEventCallback orig_keyboardEventAction_;
      OSObject* orig_keyboardEventTarget_;

      UpdateEventFlagsCallback orig_updateEventFlagsAction_;
      OSObject* orig_updateEventFlagsTarget_;

      IOLock* replacerestore_lock_;

      bool refresh_callback(void);

      /** return true if event action is replaced. */
      bool replaceEventAction(void);
      /** return true if event action is replaced. */
      bool restoreEventAction(void);
    };

    static void static_initialize(IOWorkLoop& workloop);
    static void static_terminate(void);

    static ListHookedKeyboard& instance(void);

    void apply(const Params_KeyboardEventCallBack& params);
    void apply(const Params_UpdateEventFlagsCallback& params);

  private:
    static void setCapsLockLED_callback(OSObject* owner, IOTimerEventSource* sender);
    static TimerWrapper capslock_led_timer_;
  };
}

#endif
