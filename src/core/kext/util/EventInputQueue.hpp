#ifndef EVENTINPUTQUEUE_HPP
#define EVENTINPUTQUEUE_HPP

#include "base.hpp"
#include "CallBackWrapper.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "List.hpp"
#include "ParamsUnion.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class SimultaneousButtonPresses;
    class SimultaneousKeyPresses;
  }

  class EventInputQueue {
    friend class RemapFunc::SimultaneousButtonPresses;
    friend class RemapFunc::SimultaneousKeyPresses;

  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    // ------------------------------------------------------------
    static void push_KeyboardEventCallback(OSObject* target,
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

    static void push_UpdateEventFlagsCallback(OSObject* target,
                                              unsigned flags,
                                              OSObject* sender,
                                              void* refcon);

    static void push_KeyboardSpecialEventCallback(OSObject* target,
                                                  unsigned int eventType,
                                                  unsigned int flags,
                                                  unsigned int key,
                                                  unsigned int flavor,
                                                  UInt64 guid,
                                                  bool repeat,
                                                  AbsoluteTime ts,
                                                  OSObject* sender,
                                                  void* refcon);

    static void push_RelativePointerEventCallback(OSObject* target,
                                                  int buttons,
                                                  int dx,
                                                  int dy,
                                                  AbsoluteTime ts,
                                                  OSObject* sender,
                                                  void* refcon);

    static void push_ScrollWheelEventCallback(OSObject* target,
                                              short deltaAxis1,
                                              short deltaAxis2,
                                              short deltaAxis3,
                                              IOFixed fixedDelta1,
                                              IOFixed fixedDelta2,
                                              IOFixed fixedDelta3,
                                              SInt32 pointDelta1,
                                              SInt32 pointDelta2,
                                              SInt32 pointDelta3,
                                              SInt32 options,
                                              AbsoluteTime ts,
                                              OSObject* sender,
                                              void* refcon);

    // ------------------------------------------------------------
    class Item : public List::Item {
    public:
      Item(const Params_KeyboardEventCallBack& p,        bool r, uint32_t d) :
        params(p), retainFlagStatusTemporaryCount(r), delayMS(d) {}
      Item(const Params_KeyboardSpecialEventCallback& p, bool r, uint32_t d) :
        params(p), retainFlagStatusTemporaryCount(r), delayMS(d) {}
      Item(const Params_RelativePointerEventCallback& p, bool r, uint32_t d) :
        params(p), retainFlagStatusTemporaryCount(r), delayMS(d) {}
      Item(const Params_ScrollWheelEventCallback& p,     bool r, uint32_t d) :
        params(p), retainFlagStatusTemporaryCount(r), delayMS(d) {}
      virtual ~Item(void) {}

      ParamsUnion params;
      bool retainFlagStatusTemporaryCount;

      uint32_t delayMS;
    };

  private:
    enum {
      MIN_DELAY = 1,
      IMMEDIATELY_DELAY = 1,
    };

    enum DelayType {
      DELAY_TYPE_KEY,
      DELAY_TYPE_POINTING_BUTTON,
    };

    static uint32_t calcdelay(DelayType type);

    // ------------------------------------------------------------
    static void enqueue_(const Params_KeyboardEventCallBack& p,        bool retainFlagStatusTemporaryCount, bool push_back = true);
    static void enqueue_(const Params_KeyboardSpecialEventCallback& p, bool retainFlagStatusTemporaryCount);
    static void enqueue_(const Params_RelativePointerEventCallback& p, bool retainFlagStatusTemporaryCount);
    static void enqueue_(const Params_ScrollWheelEventCallback& p,     bool retainFlagStatusTemporaryCount);
    static void fire(OSObject* owner, IOTimerEventSource* sender);
    static void setTimer(void);

    static List* queue_;
    static IntervalChecker ic_;
    static TimerWrapper timer_;
  };
}

#endif
