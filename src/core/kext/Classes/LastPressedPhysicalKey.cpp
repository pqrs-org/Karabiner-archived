#include "LastPressedPhysicalKey.hpp"
#include "ParamsUnion.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  LastPressedPhysicalKey::update(const ParamsUnion& newval)
  {
    switch (newval.type) {
      case ParamsUnion::KEYBOARD:
      {
        Params_KeyboardEventCallBack* p = newval.get_Params_KeyboardEventCallBack();

        if (p && p->ex_iskeydown) {
          datatype_ = BRIDGE_DATATYPE_KEYCODE;
          value_ = (p->key).get();
          ic_.begin();
        }
        break;
      }

      case ParamsUnion::KEYBOARD_SPECIAL:
      {
        Params_KeyboardSpecialEventCallback* p = newval.get_Params_KeyboardSpecialEventCallback();
        if (p && p->ex_iskeydown) {
          datatype_ = BRIDGE_DATATYPE_CONSUMERKEYCODE;
          value_ = (p->key).get();
          ic_.begin();
        }
        break;
      }

      case ParamsUnion::RELATIVE_POINTER:
      {
        Params_RelativePointerEventCallback* p = newval.get_Params_RelativePointerEventCallback();

        if (p && p->ex_button != PointingButton::NONE && p->ex_isbuttondown) {
          datatype_ = BRIDGE_DATATYPE_POINTINGBUTTON;
          value_ = (p->ex_button).get();
          ic_.begin();
        }
        break;
      }

      case ParamsUnion::UPDATE_FLAGS:
      case ParamsUnion::SCROLL_WHEEL:
      case ParamsUnion::WAIT:
        // Do nothing
        break;
    }
  }
}
