#ifndef CALLBACKWRAPPER_HPP
#define CALLBACKWRAPPER_HPP

#include "base.hpp"
#include "KeyCode.hpp"
#include "auto_ptr.hpp"
#include "IntervalChecker.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class Params_KeyboardEventCallBack {
  public:
    // Use auto_ptr instead allocating in kernel stack.
    DECLARE_AUTO_PTR(Params_KeyboardEventCallBack);

    static Params_KeyboardEventCallBack* alloc(EventType et, Flags fl, KeyCode kc,
                                               CharCode cc, CharSet cs, OrigCharCode occ, OrigCharSet ocs,
                                               KeyboardType kt, bool r) {
      return new Params_KeyboardEventCallBack(et, fl, kc, cc, cs, occ, ocs, kt, r);
    }
    static Params_KeyboardEventCallBack* alloc(EventType et, Flags fl, const KeyCode kc,
                                               KeyboardType kt, bool r) {
      return new Params_KeyboardEventCallBack(et, fl, kc,
                                              CharCode(0), CharSet(0), OrigCharCode(0), OrigCharSet(0),
                                              kt, r);
    }

    //~Params_KeyboardEventCallBack(void) { printf("Params_KeyboardEventCallBack released\n"); }

    // ----------------------------------------
    void log(const char* message = "caught") const;
    void apply(void) const;

    EventType eventType;
    Flags flags;
    KeyCode key;
    CharCode charCode;
    CharSet charSet;
    OrigCharCode origCharCode;
    OrigCharSet origCharSet;
    KeyboardType keyboardType;
    bool repeat;

    // ----------------------------------------
    class Queue {
    public:
      static void initialize(IOWorkLoop& workloop);
      static void terminate(void);

      static void add(const Params_KeyboardEventCallBack& p);

    private:
      enum {
        MAXNUM = 128,
        DELAY = 1,
      };
      static bool empty(void) { return current_ == last_; }

      static void fire(OSObject* owner, IOTimerEventSource* sender);
      static void fire_nolock(void);
      static Params_KeyboardEventCallBack** getnext(Params_KeyboardEventCallBack** p) {
        if (p >= item_ + (MAXNUM - 1)) {
          return item_;
        } else {
          return p + 1;
        }
      }

      static Params_KeyboardEventCallBack* item_[MAXNUM];
      static IntervalChecker ic_;
      static Params_KeyboardEventCallBack** current_;
      static Params_KeyboardEventCallBack** last_;
      static TimerWrapper timer_;
      static bool isTimerActive_;
    };

  private:
    Params_KeyboardEventCallBack(EventType et, Flags fl, KeyCode kc,
                                 CharCode cc, CharSet cs, OrigCharCode occ, OrigCharSet ocs,
                                 KeyboardType kt, bool r) :
      eventType(et), flags(fl), key(kc),
      charCode(cc), charSet(cs), origCharCode(occ), origCharSet(ocs),
      keyboardType(kt), repeat(r) {}

    void do_apply(void) const;
  };

  class Params_KeyboardSpecialEventCallback {
  public:
    // Use auto_ptr instead allocating in kernel stack.
    DECLARE_AUTO_PTR(Params_KeyboardSpecialEventCallback);

    static Params_KeyboardSpecialEventCallback* alloc(EventType et, Flags fl, ConsumerKeyCode ckc,
                                                      unsigned int fv, UInt64 g,
                                                      bool r) {
      return new Params_KeyboardSpecialEventCallback(et, fl, ckc, fv, g, r);
    }

    static Params_KeyboardSpecialEventCallback* alloc(EventType et, Flags fl, ConsumerKeyCode ckc,
                                                      bool r) {
      return new Params_KeyboardSpecialEventCallback(et, fl, ckc, ckc.get(), static_cast<UInt64>(-1), r);
    }

    //~Params_KeyboardSpecialEventCallback(void) { printf("Params_KeyboardSpecialEventCallback released\n"); }

    // ----------------------------------------
    void log(const char* message = "caught") const;
    void apply(void) const;

    EventType eventType;
    Flags flags;
    ConsumerKeyCode key;
    unsigned int flavor;
    UInt64 guid;
    bool repeat;

  private:
    Params_KeyboardSpecialEventCallback(EventType et, Flags fl, ConsumerKeyCode ckc,
                                        unsigned int fv, UInt64 g,
                                        bool r) :
      eventType(et), flags(fl), key(ckc),
      flavor(fv), guid(g),
      repeat(r) {}
  };

  class Params_RelativePointerEventCallback {
  public:
    // Use auto_ptr instead allocating in kernel stack.
    DECLARE_AUTO_PTR(Params_RelativePointerEventCallback);

    static Params_RelativePointerEventCallback* alloc(Buttons bt, int x, int y) {
      return new Params_RelativePointerEventCallback(bt, x, y);
    }

    //~Params_RelativePointerEventCallback(void) { printf("Params_RelativePointerEventCallback released\n"); }

    void log(const char* message = "caught") const;
    void apply(void) const;

    Buttons buttons;
    int dx;
    int dy;

  private:
    Params_RelativePointerEventCallback(Buttons bt, int x, int y) : buttons(bt), dx(x), dy(y) {}
  };
  class Params_ScrollWheelEventCallback {
  public:
    // Use auto_ptr instead allocating in kernel stack.
    DECLARE_AUTO_PTR(Params_ScrollWheelEventCallback);

    static Params_ScrollWheelEventCallback* alloc(short da1, short da2, short da3,
                                                  IOFixed fd1, IOFixed fd2, IOFixed fd3,
                                                  SInt32 pd1, SInt32 pd2, SInt32 pd3,
                                                  SInt32 op) {
      return new Params_ScrollWheelEventCallback(da1, da2, da3,
                                                 fd1, fd2, fd3,
                                                 pd1, pd2, pd3,
                                                 op);
    }

    //~Params_ScrollWheelEventCallback(void) { printf("Params_ScrollWheelEventCallback released\n"); }

    void log(const char* message = "caught") const;
    void apply(void) const;

    short deltaAxis1;
    short deltaAxis2;
    short deltaAxis3;
    IOFixed fixedDelta1;
    IOFixed fixedDelta2;
    IOFixed fixedDelta3;
    SInt32 pointDelta1;
    SInt32 pointDelta2;
    SInt32 pointDelta3;
    SInt32 options;

  private:
    Params_ScrollWheelEventCallback(short da1, short da2, short da3,
                                    IOFixed fd1, IOFixed fd2, IOFixed fd3,
                                    SInt32 pd1, SInt32 pd2, SInt32 pd3,
                                    SInt32 op) :
      deltaAxis1(da1), deltaAxis2(da2), deltaAxis3(da3),
      fixedDelta1(fd1), fixedDelta2(fd2), fixedDelta3(fd3),
      pointDelta1(pd1), pointDelta2(pd2), pointDelta3(pd3),
      options(op) {}
  };
}

#endif
