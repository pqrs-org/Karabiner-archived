#ifndef EVENTINPUTQUEUE_HPP
#define EVENTINPUTQUEUE_HPP

#include "base.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "RemapUtil.hpp"
#include "Queue.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace SimultaneousKeyPresses {
    namespace Option {
      enum Option {
        NORMAL,
        RAW,
      };
    };
  }

  class EventInputQueue {
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
    class Remap {
    public:
#include "../generate/output/include.keyeventinputqueue.hpp"
      void remap(void);
      bool handleVirtualKey(const Params_KeyboardEventCallBack& params);

    private:
      void push_remapped(const Params_KeyboardEventCallBack& baseparams, bool isKeyDown);

      KeyCode virtualKeyCode_;

      KeyCode fromKeyCode1_;
      KeyCode fromKeyCode2_;
      Flags fromFlags_;

      KeyCode toKeyCode1_;
      Flags toFlags1_;
      KeyCode toKeyCode2_;
      Flags toFlags2_;
      KeyCode toKeyCode3_;
      Flags toFlags3_;
      KeyCode toKeyCode4_;
      Flags toFlags4_;
      KeyCode toKeyCode5_;
      Flags toFlags5_;

      bool active1_;
      bool active2_;

      SimultaneousKeyPresses::Option::Option option_;

      RemapUtil::KeyToKey keytokey_;
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
