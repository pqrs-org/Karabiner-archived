#ifndef PARAMSUNION_HPP
#define PARAMSUNION_HPP

#include "CallBackWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ParamsUnion {
  public:
    ParamsUnion(const Params_KeyboardEventCallBack& p);
    ParamsUnion(const Params_UpdateEventFlagsCallback& p);
    ParamsUnion(const Params_KeyboardSpecialEventCallback& p);
    ParamsUnion(const Params_RelativePointerEventCallback& p);
    ParamsUnion(const Params_ScrollWheelEventCallback& p);
    ~ParamsUnion(void);

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
  };
}

#endif
