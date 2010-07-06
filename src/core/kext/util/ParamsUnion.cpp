#include "ParamsUnion.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  ParamsUnion::ParamsUnion(const Params_KeyboardEventCallBack& p) : type(KEYBOARD)
  {
    params.params_KeyboardEventCallBack = Params_KeyboardEventCallBack::alloc(p);
  }

  ParamsUnion::ParamsUnion(const Params_UpdateEventFlagsCallback& p) : type(UPDATE_FLAGS)
  {
    params.params_UpdateEventFlagsCallback = Params_UpdateEventFlagsCallback::alloc(p);
  }

  ParamsUnion::ParamsUnion(const Params_KeyboardSpecialEventCallback& p) : type(KEYBOARD_SPECIAL)
  {
    params.params_KeyboardSpecialEventCallback = Params_KeyboardSpecialEventCallback::alloc(p);
  }

  ParamsUnion::ParamsUnion(const Params_RelativePointerEventCallback& p) : type(RELATIVE_POINTER)
  {
    params.params_RelativePointerEventCallback = Params_RelativePointerEventCallback::alloc(p);
  }

  ParamsUnion::ParamsUnion(const Params_ScrollWheelEventCallback& p) : type(SCROLL_POINTER)
  {
    params.params_ScrollWheelEventCallback = Params_ScrollWheelEventCallback::alloc(p);
  }

  ParamsUnion::~ParamsUnion(void)
  {
#define DELETE_PARAMS(PARAMS) { \
    if (PARAMS) {               \
      delete PARAMS;            \
    }                           \
}

    switch (type) {
      case KEYBOARD:
        DELETE_PARAMS(params.params_KeyboardEventCallBack);
        break;
      case UPDATE_FLAGS:
        DELETE_PARAMS(params.params_UpdateEventFlagsCallback);
        break;
      case KEYBOARD_SPECIAL:
        DELETE_PARAMS(params.params_KeyboardSpecialEventCallback);
        break;
      case RELATIVE_POINTER:
        DELETE_PARAMS(params.params_RelativePointerEventCallback);
        break;
      case SCROLL_POINTER:
        DELETE_PARAMS(params.params_ScrollWheelEventCallback);
        break;
      default:
        IOLOG_ERROR("ParamsUnion::~ParamsUnion() unknown type\n");
    }

#undef DELETE_PARAMS
  }
}
