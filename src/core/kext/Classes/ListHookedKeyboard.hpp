#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/hidsystem/IOHIKeyboard.h>
END_IOKIT_INCLUDE;

#include "ListHookedDevice.hpp"

namespace org_pqrs_Karabiner {
class Params_KeyboardEventCallBack;
class Params_UpdateEventFlagsCallback;
class TimerWrapper;

class ListHookedKeyboard final : public ListHookedDevice {
public:
  class Item final : public ListHookedDevice::Item {
  public:
    Item(IOHIDevice* p);
    ~Item(void);

    bool isReplaced(void) const override { return orig_keyboardEventAction_ != nullptr; }

    void apply(const Params_KeyboardEventCallBack& params);
    void apply(const Params_UpdateEventFlagsCallback& params);

  private:
    KeyboardEventCallback orig_keyboardEventAction_;
    OSObject* orig_keyboardEventTarget_;

    UpdateEventFlagsCallback orig_updateEventFlagsAction_;
    OSObject* orig_updateEventFlagsTarget_;

    bool refresh(void) override;

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

  bool isExternalDevicesConnected(void) const;

private:
  static void setcapslock_timer_callback(OSObject* owner, IOTimerEventSource* sender);
  static TimerWrapper setcapslock_timer_;
};
}

#endif
