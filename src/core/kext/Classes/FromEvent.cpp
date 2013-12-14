#include "FromEvent.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  FromEvent::isTargetEvent(bool& isDown, const ParamsUnion& paramsUnion) const
  {
    switch (type_) {
      case Type::NONE:
      {
        return false;
      }

      case Type::KEY:
      {
        if (paramsUnion.type != ParamsUnion::KEYBOARD) return false;

        Params_KeyboardEventCallBack* params = paramsUnion.params.params_KeyboardEventCallBack;
        if (! params) return false;

        if (params->key != key_) return false;

        isDown = params->ex_iskeydown;
        return true;
      }

      case Type::CONSUMER_KEY:
      {
        if (paramsUnion.type != ParamsUnion::KEYBOARD_SPECIAL) return false;

        Params_KeyboardSpecialEventCallback* params = paramsUnion.params.params_KeyboardSpecialEventCallback;
        if (! params) return false;

        if (params->key != consumer_) return false;

        isDown = params->ex_iskeydown;
        return true;
      }

      case Type::POINTING_BUTTON:
      {
        if (paramsUnion.type != ParamsUnion::RELATIVE_POINTER) return false;

        Params_RelativePointerEventCallback* params = paramsUnion.params.params_RelativePointerEventCallback;
        if (! params) return false;

        if (params->ex_button != button_) return false;

        isDown = params->ex_isbuttondown;
        return true;
      }
    }

    return false;
  }

  bool
  FromEvent::isTargetDownEvent(const ParamsUnion& paramsUnion)
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, paramsUnion)) return false;
    if (! isDown) return false;

    isPressing_ = true;
    return true;
  }

  bool
  FromEvent::isTargetUpEvent(const ParamsUnion& paramsUnion)
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, paramsUnion)) return false;
    if (isDown) return false;

    isPressing_ = false;
    return true;
  }
}
