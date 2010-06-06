#ifndef CALLBACKWRAPPER_HPP
#define CALLBACKWRAPPER_HPP

#include "base.hpp"
#include "KeyCode.hpp"
#include "auto_ptr.hpp"
#include "IntervalChecker.hpp"
#include "TimerWrapper.hpp"
#include "Queue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class Params_KeyboardEventCallBack {
    friend class CallBackWrapperQueue;

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

  private:
    Params_KeyboardEventCallBack(EventType et, Flags fl, KeyCode kc,
                                 CharCode cc, CharSet cs, OrigCharCode occ, OrigCharSet ocs,
                                 KeyboardType kt, bool r) :
      eventType(et), flags(fl), key(kc),
      charCode(cc), charSet(cs), origCharCode(occ), origCharSet(ocs),
      keyboardType(kt), repeat(r) {}

    void do_apply(void) const;
  };

  class Params_UpdateEventFlagsCallback {
    friend class CallBackWrapperQueue;

  public:
    // Use auto_ptr instead allocating in kernel stack.
    DECLARE_AUTO_PTR(Params_UpdateEventFlagsCallback);

    static Params_UpdateEventFlagsCallback* alloc(Flags fl) {
      return new Params_UpdateEventFlagsCallback(fl);
    }

    // ----------------------------------------
    void log(const char* message = "caught") const;
    void apply(void) const;
    Flags flags;

  private:
    Params_UpdateEventFlagsCallback(Flags fl) : flags(fl) {}
  };

  class Params_KeyboardSpecialEventCallback {
    friend class CallBackWrapperQueue;

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
    friend class CallBackWrapperQueue;

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
    friend class CallBackWrapperQueue;

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

  // ======================================================================
  class CallBackWrapperQueue {
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
      enum Type {
        KEYBOARD,
        UPDATE_FLAGS,
        KEYBOARD_SPECIAL,
        RELATIVE_POINTER,
        SCROLL_POINTER,
      } type;

      union {
        Params_KeyboardEventCallBack* params_KeyboardEventCallBack;
        Params_UpdateEventFlagsCallback* params_UpdateEventFlagsCallback;
        Params_KeyboardSpecialEventCallback* params_KeyboardSpecialEventCallback;
        Params_RelativePointerEventCallback* params_RelativePointerEventCallback;
        Params_ScrollWheelEventCallback* params_ScrollWheelEventCallback;
      } params;

      virtual ~Item(void);
    };

    static void fire(OSObject* owner, IOTimerEventSource* sender);
    static void fire_nolock(void);
    static void push(Item& p);

    static Queue *queue_;
    static IntervalChecker ic_;
    static TimerWrapper timer_;
    static bool isTimerActive_;
  };
}

#endif
