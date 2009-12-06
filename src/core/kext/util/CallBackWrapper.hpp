#ifndef CALLBACKWRAPPER_HPP
#define CALLBACKWRAPPER_HPP

#include "base.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class Params_KeyboardEventCallBack {
  public:
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
    void log(const char* message = "caught") const;
    void apply(void) const;

    OSObject* target;
    EventType eventType;
    Flags flags;
    ConsumerKeyCode key;
    unsigned int flavor;
    UInt64 guid;
    bool repeat;
    AbsoluteTime ts;
    OSObject* sender;
    void* refcon;
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
