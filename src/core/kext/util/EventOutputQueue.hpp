#ifndef EVENTOUTPUTQUEUE_HPP
#define EVENTOUTPUTQUEUE_HPP

#include "IntervalChecker.hpp"
#include "ParamsUnion.hpp"
#include "Queue.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class EventOutputQueue {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static void push(const Params_KeyboardEventCallBack& p);
    static void push(const Params_UpdateEventFlagsCallback& p);
    static void push(const Params_KeyboardSpecialEventCallback& p);
    static void push(const Params_RelativePointerEventCallback& p);
    static void push(const Params_ScrollWheelEventCallback& p);

  private:
    enum {
      DELAY = 1,
    };
    class Item : public Queue::Item {
    public:
      Item(const Params_KeyboardEventCallBack& p)        : params(p) {}
      Item(const Params_UpdateEventFlagsCallback& p)     : params(p) {}
      Item(const Params_KeyboardSpecialEventCallback& p) : params(p) {}
      Item(const Params_RelativePointerEventCallback& p) : params(p) {}
      Item(const Params_ScrollWheelEventCallback& p)     : params(p) {}

      virtual ~Item(void) {}

      ParamsUnion params;
    };

    static void fire(OSObject* owner, IOTimerEventSource* sender);
    static void fire_nolock(void);
    static void push(Item& p);

    static Queue* queue_;
    static IntervalChecker ic_;
    static TimerWrapper timer_;
  };
}

#endif
