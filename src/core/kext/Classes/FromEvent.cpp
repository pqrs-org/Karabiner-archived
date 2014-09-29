#include "FromEvent.hpp"

namespace org_pqrs_Karabiner {
  bool
  FromEvent::isTargetEvent(bool& isDown, const Params_Base& paramsBase) const
  {
    switch (type_) {
      case Type::NONE:
      {
        return false;
      }

      case Type::KEY:
      {
        auto p = paramsBase.get_Params_KeyboardEventCallBack();
        if (! p) return false;

        if (p->key != key_) return false;

        isDown = p->ex_iskeydown;
        return true;
      }

      case Type::CONSUMER_KEY:
      {
        auto p = paramsBase.get_Params_KeyboardSpecialEventCallback();
        if (! p) return false;

        if (p->key != consumer_) return false;

        isDown = p->ex_iskeydown;
        return true;
      }

      case Type::POINTING_BUTTON:
      {
        auto p = paramsBase.get_Params_RelativePointerEventCallback();
        if (! p) return false;

        if (p->ex_button != button_) return false;

        isDown = p->ex_isbuttondown;
        return true;
      }
    }

    return false;
  }

  bool
  FromEvent::changePressingState(const Params_Base& paramsBase,
                                 const FlagStatus& currentFlags,
                                 const Vector_ModifierFlag& fromFlags)
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, paramsBase)) return false;

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
  FromEvent::isTargetDownEvent(const Params_Base& paramsBase) const
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, paramsBase)) return false;
    return isDown;
  }

  bool
  FromEvent::isTargetUpEvent(const Params_Base& paramsBase) const
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, paramsBase)) return false;
    return ! isDown;
  }
}
