#ifndef EVENTINPUTQUEUE_HPP
#define EVENTINPUTQUEUE_HPP

#include "base.hpp"
#include "CallBackWrapper.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "Queue.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class SimultaneousKeyPresses;
  }

  class EventInputQueue {
    friend class RemapFunc::SimultaneousKeyPresses;

  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static void push(const Params_KeyboardEventCallBack& p);

    class Item : public Queue::Item {
    public:
      Item(const Params_KeyboardEventCallBack& p, uint32_t d);
      virtual ~Item(void);

      Params_KeyboardEventCallBack* params_KeyboardEventCallBack;

      bool dropped;
      uint32_t delayMS;
    };

  private:
    enum {
      MIN_DELAY = 1,
    };

    static void enqueue_(const Params_KeyboardEventCallBack& p);
    static void fire(OSObject* owner, IOTimerEventSource* sender);
    static void fire_nolock(void);
    static void setTimer(void);

    static Queue* queue_;
    static IntervalChecker ic_;
    static TimerWrapper timer_;
  };
}

#endif
