#ifndef KEYEVENTINPUTQUEUE_HPP
#define KEYEVENTINPUTQUEUE_HPP

#include "base.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyEventInputQueue {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static void add(OSObject* target,
                    EventType eventType,
                    Flags flags,
                    KeyCode key,
                    CharCode charCode,
                    CharSet charSet,
                    OrigCharCode origCharCode,
                    OrigCharSet origCharSet,
                    KeyboardType keyboardType,
                    bool repeat,
                    AbsoluteTime ts,
                    OSObject* sender,
                    void* refcon);

    class Remap {
    public:
      Remap(void) : active1_(false), active2_(false) {}
      void remap(KeyCode fromKeyCode1, KeyCode fromKeyCode2, KeyCode toKeyCode);

    private:
      bool active1_;
      bool active2_;
    };

  private:
    enum {
      MAXNUM = 32,
    };
    struct Item {
      OSObject* target;
      EventType eventType;
      Flags flags;
      KeyCode key;
      CharCode charCode;
      CharSet charSet;
      OrigCharCode origCharCode;
      OrigCharSet origCharSet;
      KeyboardType keyboardType;
      bool repeat;
      AbsoluteTime ts;
      OSObject* sender;
      void* refcon;

      bool active;
      bool dropped;
      bool isremapped;
      UInt32 delayMS;
    };

    static void fire(OSObject* owner, IOTimerEventSource* sender);
    static void fire_nolock(void);
    static Item* getnext(Item* p) {
      if (p >= item_ + (MAXNUM - 1)) {
        return item_;
      } else {
        return p + 1;
      }
    }

    static Item item_[MAXNUM];
    static IntervalChecker ic_;
    static Item* current_;
    static Item* last_;
    static TimerWrapper timer_;
    static bool isTimerActive_;
  };
}

#endif
