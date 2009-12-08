#ifndef CALLBACKWRAPPER_HPP
#define CALLBACKWRAPPER_HPP

#include "base.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class Params_KeyboardEventCallBack {
  public:
    explicit Params_KeyboardEventCallBack(const EventType& et, const Flags& fl, const KeyCode& kc,
                                          unsigned int cc,
                                          unsigned int cs,
                                          unsigned int occ,
                                          unsigned int ocs,
                                          const KeyboardType& kt, bool r) :
      eventType(et), flags(fl), key(kc),
      charCode(cc), charSet(cs), origCharCode(occ), origCharSet(ocs),
      keyboardType(kt), repeat(r) {}

    explicit Params_KeyboardEventCallBack(const EventType& et, const Flags& fl, const KeyCode& kc,
                                          const KeyboardType& kt, bool r) :
      eventType(et), flags(fl), key(kc),
      charCode(0), charSet(0), origCharCode(0), origCharSet(0),
      keyboardType(kt), repeat(r) {}

    // ----------------------------------------
    void log(const char* message = "caught") const;
    void apply(void) const;

    EventType eventType;
    Flags flags;
    KeyCode key;
    unsigned int charCode;
    unsigned int charSet;
    unsigned int origCharCode;
    unsigned int origCharSet;
    KeyboardType keyboardType;
    bool repeat;

  private:
    static KeyboardType current_keyboardType_;
  };

  class Params_KeyboardSpecialEventCallback {
  public:
    explicit Params_KeyboardSpecialEventCallback(const EventType& et, const Flags& fl, const ConsumerKeyCode& ckc,
                                                 unsigned int fv, UInt64 g,
                                                 bool r) :
      eventType(et), flags(fl), key(ckc),
      flavor(fv), guid(g),
      repeat(r) {}

    explicit Params_KeyboardSpecialEventCallback(const EventType& et, const Flags& fl, const ConsumerKeyCode& ckc,
                                                 bool r) :
      eventType(et), flags(fl), key(ckc),
      flavor(ckc.get()), guid(static_cast<UInt64>(-1)),
      repeat(r) {}

    // ----------------------------------------
    void log(const char* message = "caught") const;
    void apply(void) const;

    EventType eventType;
    Flags flags;
    ConsumerKeyCode key;
    unsigned int flavor;
    UInt64 guid;
    bool repeat;
  };

  class Params_RelativePointerEventCallback {
  public:
    void log(const char* message = "caught") const;
    void apply(void) const;

    OSObject* target;
    Buttons buttons;
    int dx;
    int dy;
    AbsoluteTime ts;
    OSObject* sender;
    void* refcon;
  };
  class Params_ScrollWheelEventCallback {
  public:
    void log(const char* message = "caught") const;
    void apply(void) const;

    OSObject* target;
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
    AbsoluteTime ts;
    OSObject* sender;
    void* refcon;
  };
}

#endif
