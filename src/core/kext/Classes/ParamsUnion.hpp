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
    };
    const Type type;

    Params_KeyboardEventCallBack* get_Params_KeyboardEventCallBack(void) const {
      if (type != KEYBOARD) return NULL;
      return params_.params_KeyboardEventCallBack;
    }
    Params_UpdateEventFlagsCallback* get_Params_UpdateEventFlagsCallback(void) const {
      if (type != UPDATE_FLAGS) return NULL;
      return params_.params_UpdateEventFlagsCallback;
    }
    Params_KeyboardSpecialEventCallback* get_Params_KeyboardSpecialEventCallback(void) const {
      if (type != KEYBOARD_SPECIAL) return NULL;
      return params_.params_KeyboardSpecialEventCallback;
    }
    Params_RelativePointerEventCallback* get_Params_RelativePointerEventCallback(void) const {
      if (type != RELATIVE_POINTER) return NULL;
      return params_.params_RelativePointerEventCallback;
    }
    Params_ScrollWheelEventCallback* get_Params_ScrollWheelEventCallback(void) const {
      if (type != SCROLL_WHEEL) return NULL;
      return params_.params_ScrollWheelEventCallback;
    }
    Params_Wait* get_Params_Wait(void) const {
      if (type != WAIT) return NULL;
      return params_.params_Wait;
    }

    bool iskeydown(bool& output) const {
      output = false;

      switch (type) {
        case KEYBOARD: {
          Params_KeyboardEventCallBack* p = get_Params_KeyboardEventCallBack();
          if (p) {
            output = p->ex_iskeydown;
            return true;
          }
          break;
        }

        case KEYBOARD_SPECIAL: {
          Params_KeyboardSpecialEventCallback* p = get_Params_KeyboardSpecialEventCallback();
          if (p) {
            output = p->ex_iskeydown;
            return true;
          }
          break;
        }

        case RELATIVE_POINTER: {
          Params_RelativePointerEventCallback* p = get_Params_RelativePointerEventCallback();
          if (p) {
            output = p->ex_isbuttondown;
            return true;
          }
          break;
        }

        case UPDATE_FLAGS:
        case SCROLL_WHEEL:
        case WAIT:
          break;
      }

      return false;
    }

  private:
    union {
      Params_KeyboardEventCallBack* params_KeyboardEventCallBack;
      Params_UpdateEventFlagsCallback* params_UpdateEventFlagsCallback;
      Params_KeyboardSpecialEventCallback* params_KeyboardSpecialEventCallback;
      Params_RelativePointerEventCallback* params_RelativePointerEventCallback;
      Params_ScrollWheelEventCallback* params_ScrollWheelEventCallback;
      Params_Wait* params_Wait;
    } params_;
  };
}

#endif
