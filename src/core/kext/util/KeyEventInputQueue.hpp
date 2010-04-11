#ifndef KEYEVENTINPUTQUEUE_HPP
#define KEYEVENTINPUTQUEUE_HPP

#include "base.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyEventInputQueue {
  public:
    class Remap;
    friend class Remap;

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

    class Remap {
    public:
      Remap(void) : active1_(false), active2_(false) {}
      void push(Item* base, KeyCode key);

      void remap(KeyCode fromKeyCode1, KeyCode fromKeyCode2, KeyCode toKeyCode1, KeyCode toKeyCode2, KeyCode toKeyCode3, KeyCode toKeyCode4, KeyCode toKeyCode5);
      void remap(KeyCode fromKeyCode1, KeyCode fromKeyCode2, KeyCode toKeyCode1, KeyCode toKeyCode2, KeyCode toKeyCode3, KeyCode toKeyCode4) {
        remap(fromKeyCode1, fromKeyCode2, toKeyCode1, toKeyCode2, toKeyCode3, toKeyCode4, KeyCode::VK_NONE);
      }
      void remap(KeyCode fromKeyCode1, KeyCode fromKeyCode2, KeyCode toKeyCode1, KeyCode toKeyCode2, KeyCode toKeyCode3) {
        remap(fromKeyCode1, fromKeyCode2, toKeyCode1, toKeyCode2, toKeyCode3, KeyCode::VK_NONE);
      }
      void remap(KeyCode fromKeyCode1, KeyCode fromKeyCode2, KeyCode toKeyCode1, KeyCode toKeyCode2) {
        remap(fromKeyCode1, fromKeyCode2, toKeyCode1, toKeyCode2, KeyCode::VK_NONE);
      }
      void remap(KeyCode fromKeyCode1, KeyCode fromKeyCode2, KeyCode toKeyCode1) {
        remap(fromKeyCode1, fromKeyCode2, toKeyCode1, KeyCode::VK_NONE);
      }

    private:
      bool active1_;
      bool active2_;
    };

  private:
    enum {
      MAXNUM = 64,
    };

    static Item* enqueue_(OSObject* target,
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
