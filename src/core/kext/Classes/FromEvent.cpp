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
        Params_KeyboardEventCallBack* params = paramsUnion.get_Params_KeyboardEventCallBack();
        if (! params) return false;

        if (params->key != key_) return false;

        isDown = params->ex_iskeydown;
        return true;
      }

      case Type::CONSUMER_KEY:
      {
        Params_KeyboardSpecialEventCallback* params = paramsUnion.get_Params_KeyboardSpecialEventCallback();
        if (! params) return false;

        if (params->key != consumer_) return false;

        isDown = params->ex_iskeydown;
        return true;
      }

      case Type::POINTING_BUTTON:
      {
        Params_RelativePointerEventCallback* params = paramsUnion.get_Params_RelativePointerEventCallback();
        if (! params) return false;

        if (params->ex_button != button_) return false;

        isDown = params->ex_isbuttondown;
        return true;
      }
    }

    return false;
  }

  bool
  FromEvent::changePressingState(const ParamsUnion& paramsUnion, Flags currentFlags, Flags fromFlags)
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, paramsUnion)) return false;

    if (isDown) {
      if (currentFlags.isOn(fromFlags)) {
        isPressing_ = true;
        return true;
      }

    } else {
      if (isPressing_) {
        isPressing_ = false;
        return true;
      }
    }

    return false;
  }

  bool
  FromEvent::isTargetDownEvent(const ParamsUnion& paramsUnion) const
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, paramsUnion)) return false;
    return isDown;
  }

  bool
  FromEvent::isTargetUpEvent(const ParamsUnion& paramsUnion) const
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, paramsUnion)) return false;
    return ! isDown;
  }
}
