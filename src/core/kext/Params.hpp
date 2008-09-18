#ifndef PARAMS_HPP
#define PARAMS_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  struct Params_KeyboardEventCallBack {
    OSObject *target;
    unsigned int eventType;
    unsigned int flags;
    unsigned int key;
    unsigned int charCode;
    unsigned int charSet;
    unsigned int origCharCode;
    unsigned int origCharSet;
    unsigned int keyboardType;
    bool repeat;
    AbsoluteTime ts;
    OSObject *sender;
    void *refcon;
  };
  struct Params_KeyboardSpecialEventCallback {
    OSObject *target;
    unsigned int eventType;
    unsigned int flags;
    unsigned int key;
    unsigned int flavor;
    UInt64 guid;
    bool repeat;
    AbsoluteTime ts;
    OSObject *sender;
    void *refcon;
  };
  struct Params_RelativePointerEventCallback {
    OSObject *target;
    int buttons;
    int dx;
    int dy;
    AbsoluteTime ts;
    OSObject *sender;
    void *refcon;
  };
  struct Params_ScrollWheelEventCallback {
    OSObject * target;
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
    OSObject *sender;
    void *refcon;
  };
}

#endif
