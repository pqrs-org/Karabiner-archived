#ifndef CALLBACKWRAPPER_HPP
#define CALLBACKWRAPPER_HPP

#include "auto_ptr.hpp"
#include "base.hpp"
#include "CommonData.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class Params_KeyboardEventCallBack {
    friend class EventOutputQueue;

  public:
    ~Params_KeyboardEventCallBack(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack.
    DECLARE_AUTO_PTR(Params_KeyboardEventCallBack);

    static Params_KeyboardEventCallBack* alloc(EventType et, Flags fl, KeyCode kc,
                                               CharCode cc, CharSet cs, OrigCharCode occ, OrigCharSet ocs,
                                               KeyboardType kt, bool r) {
      return new Params_KeyboardEventCallBack(et, fl, kc, cc, cs, occ, ocs, kt, r);
    }
    static Params_KeyboardEventCallBack* alloc(EventType et, Flags fl, KeyCode kc,
                                               KeyboardType kt, bool r) {
      return new Params_KeyboardEventCallBack(et, fl, kc,
                                              CharCode(0), CharSet(0), OrigCharCode(0), OrigCharSet(0),
                                              kt, r);
    }
    static Params_KeyboardEventCallBack* alloc(const Params_KeyboardEventCallBack& p) {
      return new Params_KeyboardEventCallBack(p.eventType, p.flags, p.key,
                                              p.charCode, p.charSet, p.origCharCode, p.origCharSet,
                                              p.keyboardType, p.repeat);
    }

    // ----------------------------------------
    void log(const char* message = "caught") const;

    const EventType eventType;
    const Flags flags;
    const KeyCode key;
    const CharCode charCode;
    const CharSet charSet;
    const OrigCharCode origCharCode;
    const OrigCharSet origCharSet;
    const KeyboardType keyboardType;
    const bool repeat;

    const bool ex_iskeydown;

  private:
    Params_KeyboardEventCallBack(EventType et, Flags fl, KeyCode kc,
                                 CharCode cc, CharSet cs, OrigCharCode occ, OrigCharSet ocs,
                                 KeyboardType kt, bool r) :
      eventType(et), flags(fl), key(kc),
      charCode(cc), charSet(cs), origCharCode(occ), origCharSet(ocs),
      keyboardType(kt), repeat(r), ex_iskeydown(et.isKeyDownOrModifierDown(kc, fl)) {
      CommonData::increase_alloccount();
    }
  };

  class Params_UpdateEventFlagsCallback {
    friend class EventOutputQueue;

  public:
    ~Params_UpdateEventFlagsCallback(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack.
    DECLARE_AUTO_PTR(Params_UpdateEventFlagsCallback);

    static Params_UpdateEventFlagsCallback* alloc(Flags fl) {
      return new Params_UpdateEventFlagsCallback(fl);
    }
    static Params_UpdateEventFlagsCallback* alloc(const Params_UpdateEventFlagsCallback& p) {
      return new Params_UpdateEventFlagsCallback(p.flags);
    }

    // ----------------------------------------
    void log(const char* message = "caught") const;
    const Flags flags;

  private:
    Params_UpdateEventFlagsCallback(Flags fl) : flags(fl) {
      CommonData::increase_alloccount();
    }
  };

  class Params_KeyboardSpecialEventCallback {
    friend class EventOutputQueue;

  public:
    ~Params_KeyboardSpecialEventCallback(void) {
      CommonData::decrease_alloccount();
    }

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

    static Params_KeyboardSpecialEventCallback* alloc(const Params_KeyboardSpecialEventCallback& p) {
      return new Params_KeyboardSpecialEventCallback(p.eventType,
                                                     p.flags,
                                                     p.key,
                                                     p.flavor,
                                                     p.guid,
                                                     p.repeat);
    }

    // ----------------------------------------
    void log(const char* message = "caught") const;

    const EventType eventType;
    const Flags flags;
    const ConsumerKeyCode key;
    const unsigned int flavor;
    const UInt64 guid;
    const bool repeat;

    const bool ex_iskeydown;

  private:
    Params_KeyboardSpecialEventCallback(EventType et, Flags fl, ConsumerKeyCode ckc,
                                        unsigned int fv, UInt64 g,
                                        bool r) :
      eventType(et), flags(fl), key(ckc),
      flavor(fv), guid(g),
      repeat(r), ex_iskeydown(et == EventType::DOWN) {
      CommonData::increase_alloccount();
    }
  };

  class Params_RelativePointerEventCallback {
    friend class EventOutputQueue;

  public:
    ~Params_RelativePointerEventCallback(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack.
    DECLARE_AUTO_PTR(Params_RelativePointerEventCallback);

    static Params_RelativePointerEventCallback* alloc(Buttons bt, int x, int y, PointingButton ex_btn, bool ex_isdown) {
      return new Params_RelativePointerEventCallback(bt, x, y, ex_btn, ex_isdown);
    }

    static Params_RelativePointerEventCallback* alloc(const Params_RelativePointerEventCallback& p) {
      return new Params_RelativePointerEventCallback(p.buttons, p.dx, p.dy, p.ex_button, p.ex_isbuttondown);
    }

    void log(const char* message = "caught") const;

    const Buttons buttons;
    const int dx;
    const int dy;

    const PointingButton ex_button;
    const bool ex_isbuttondown;

  private:
    Params_RelativePointerEventCallback(Buttons bt, int x, int y, PointingButton ex_btn, bool ex_isdown) : buttons(bt), dx(x), dy(y), ex_button(ex_btn), ex_isbuttondown(ex_isdown) {
      CommonData::increase_alloccount();
    }
  };
  class Params_ScrollWheelEventCallback {
    friend class EventOutputQueue;

  public:
    ~Params_ScrollWheelEventCallback(void) {
      CommonData::decrease_alloccount();
    }

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

    static Params_ScrollWheelEventCallback* alloc(const Params_ScrollWheelEventCallback& p) {
      return new Params_ScrollWheelEventCallback(p.deltaAxis1,
                                                 p.deltaAxis2,
                                                 p.deltaAxis3,
                                                 p.fixedDelta1,
                                                 p.fixedDelta2,
                                                 p.fixedDelta3,
                                                 p.pointDelta1,
                                                 p.pointDelta2,
                                                 p.pointDelta3,
                                                 p.options);
    }

    void log(const char* message = "caught") const;

    const short deltaAxis1;
    const short deltaAxis2;
    const short deltaAxis3;
    const IOFixed fixedDelta1;
    const IOFixed fixedDelta2;
    const IOFixed fixedDelta3;
    const SInt32 pointDelta1;
    const SInt32 pointDelta2;
    const SInt32 pointDelta3;
    const SInt32 options;

  private:
    Params_ScrollWheelEventCallback(short da1, short da2, short da3,
                                    IOFixed fd1, IOFixed fd2, IOFixed fd3,
                                    SInt32 pd1, SInt32 pd2, SInt32 pd3,
                                    SInt32 op) :
      deltaAxis1(da1), deltaAxis2(da2), deltaAxis3(da3),
      fixedDelta1(fd1), fixedDelta2(fd2), fixedDelta3(fd3),
      pointDelta1(pd1), pointDelta2(pd2), pointDelta3(pd3),
      options(op) {
      CommonData::increase_alloccount();
    }
  };
}

#endif
