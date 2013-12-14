#include <IOKit/IOLib.h>

#include "EventInputQueue.hpp"
#include "FromEvent.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  FromEvent::isTargetEvent(bool& isDown, const EventInputQueue::Item& item) const
  {
    switch (type_) {
      case Type::NONE:
      {
        return false;
      }

      case Type::KEY:
      {
        if (item.params.type != ParamsUnion::KEYBOARD) return false;

        Params_KeyboardEventCallBack* params = item.params.params.params_KeyboardEventCallBack;
        if (! params) return false;

        if (params->key != key_) return false;

        isDown = params->ex_iskeydown;
        return true;
      }

      case Type::CONSUMER_KEY:
      {
        if (item.params.type != ParamsUnion::KEYBOARD_SPECIAL) return false;

        Params_KeyboardSpecialEventCallback* params = item.params.params.params_KeyboardSpecialEventCallback;
        if (! params) return false;

        if (params->key != consumer_) return false;

        isDown = params->ex_iskeydown;
        return true;
      }

      case Type::POINTING_BUTTON:
      {
        if (item.params.type != ParamsUnion::RELATIVE_POINTER) return false;

        Params_RelativePointerEventCallback* params = item.params.params.params_RelativePointerEventCallback;
        if (! params) return false;

        if (params->ex_button != button_) return false;

        isDown = params->ex_isbuttondown;
        return true;
      }
    }

    return false;
  }

  bool
  FromEvent::isTargetDownEvent(const EventInputQueue::Item& item) const
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, item)) return false;
    return isDown;
  }

  bool
  FromEvent::isTargetUpEvent(const EventInputQueue::Item& item) const
  {
    bool isDown = false;
    if (! isTargetEvent(isDown, item)) return false;
    return ! isDown;
  }
}
