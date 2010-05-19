#ifndef KEYEVENTINPUTQUEUE_HPP
#define KEYEVENTINPUTQUEUE_HPP

#include "base.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "RemapUtil.hpp"
#include "TimerWrapper.hpp"
#include "../config/output/include.remapcode_info.cpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyEventInputQueue {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static bool handleVirtualKey(const Params_KeyboardEventCallBack& params);

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
      UInt32 delayMS;
    };

    class Remap {
    public:
#include "../generate/output/include.keyeventinputqueue.hpp"
      void push(Item* base, KeyCode key, bool isKeyDown);
      void remap(void);
      bool handleVirtualKey(const Params_KeyboardEventCallBack& params);

    private:
      KeyCode virtualKeyCode_;

      KeyCode fromKeyCode1_;
      KeyCode fromKeyCode2_;

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

      RemapUtil::KeyToKey keytokey_;
    };

    class RemapClass {
    public:
      virtual void initialize(void) = 0;
      virtual void remap(void) = 0;
      virtual bool handleVirtualKey(const Params_KeyboardEventCallBack& params) = 0;
      virtual bool enabled(void) const = 0;
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
    static Item* getprev(Item* p) {
      if (p <= item_) {
        return item_ + (MAXNUM - 1);
      } else {
        return p - 1;
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
