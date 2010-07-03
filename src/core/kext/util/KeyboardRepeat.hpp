#ifndef KEYBOARDREPEAT_HPP
#define KEYBOARDREPEAT_HPP

#include "base.hpp"
#include "bridge.hpp"
#include "CallbackWrapper.hpp"
#include "Config.hpp"
#include "KeyCode.hpp"
#include "Queue.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyboardRepeat {
  public:
    KeyboardRepeat(void) {}

    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static void cancel(void);

    // --------------------------------------------------
    static void set(EventType eventType,
                    Flags flags,
                    KeyCode key,
                    KeyboardType keyboardType,
                    int wait);

    static void set(const Params_KeyboardEventCallBack& p,
                    int wait = config.get_repeat_initial_wait()) {
      set(p.eventType, p.flags, p.key, p.keyboardType, wait);
    }

    // for consumer
    static void set(EventType eventType,
                    Flags flags,
                    ConsumerKeyCode key);
    static void set(const Params_KeyboardSpecialEventCallback& p) {
      set(p.eventType, p.flags, p.key);
    }

    // --------------------------------------------------
    // primitive operations.
    static void primitive_add(EventType eventType,
                              Flags flags,
                              KeyCode key,
                              KeyboardType keyboardType);

    static void primitive_add(EventType eventType,
                              Flags flags,
                              ConsumerKeyCode key);

    static void primitive_start(int wait = config.get_repeat_initial_wait());

  private:
    class Item : public Queue::Item {
    public:
      enum Type {
        TYPE_KEYBOARD,
        TYPE_CONSUMER,
      } type;

      union {
        Params_KeyboardEventCallBack* params_KeyboardEventCallBack;
        Params_KeyboardSpecialEventCallback* params_KeyboardSpecialEventCallback;
      } params;

      virtual ~Item(void);
    };

    // ------------------------------------------------------------
    static void fire(OSObject* owner, IOTimerEventSource* sender);

    static void clear_queue(void);
    static void cancel_nolock(void);

    static void primitive_add_nolock(EventType eventType,
                                     Flags flags,
                                     KeyCode key,
                                     KeyboardType keyboardType);
    static void primitive_add_nolock(EventType eventType,
                                     Flags flags,
                                     ConsumerKeyCode key);
    static void primitive_start_nolock(int wait);

    static Queue* queue_;
    static TimerWrapper timer_;
  };
}

#endif
