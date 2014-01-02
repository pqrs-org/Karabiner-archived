#ifndef PARAMSUNION_HPP
#define PARAMSUNION_HPP

#include "CallBackWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ParamsUnion {
  public:
    explicit ParamsUnion(const Params_KeyboardEventCallBack& p);
    explicit ParamsUnion(const Params_UpdateEventFlagsCallback& p);
    explicit ParamsUnion(const Params_KeyboardSpecialEventCallback& p);
    explicit ParamsUnion(const Params_RelativePointerEventCallback& p);
    explicit ParamsUnion(const Params_ScrollWheelEventCallback& p);
    explicit ParamsUnion(const Params_Wait& p);
    ~ParamsUnion(void);

    enum Type {
      KEYBOARD,
      UPDATE_FLAGS,
      KEYBOARD_SPECIAL,
      RELATIVE_POINTER,
      SCROLL_WHEEL,
      WAIT,
    } type;

    union {
      Params_KeyboardEventCallBack* params_KeyboardEventCallBack;
      Params_UpdateEventFlagsCallback* params_UpdateEventFlagsCallback;
      Params_KeyboardSpecialEventCallback* params_KeyboardSpecialEventCallback;
      Params_RelativePointerEventCallback* params_RelativePointerEventCallback;
      Params_ScrollWheelEventCallback* params_ScrollWheelEventCallback;
      Params_Wait* params_Wait;
    } params;
  };
}

#endif
