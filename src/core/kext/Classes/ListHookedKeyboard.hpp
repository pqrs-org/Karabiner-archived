#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include <IOKit/hidsystem/IOHIKeyboard.h>

#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class Params_KeyboardEventCallBack;
  class Params_UpdateEventFlagsCallback;
  class TimerWrapper;

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

      bool refresh(void);

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
    static void setcapslock_timer_callback(OSObject* owner, IOTimerEventSource* sender);
    static TimerWrapper setcapslock_timer_;
  };
}

#endif
