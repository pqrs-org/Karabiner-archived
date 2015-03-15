#include "LastPressedPhysicalKey.hpp"
#include "Params.hpp"

namespace org_pqrs_Karabiner {
void LastPressedPhysicalKey::update(const Params_Base& newval) {
  {
    auto p = newval.get_Params_KeyboardEventCallBack();

    if (p && p->ex_iskeydown) {
      datatype_ = BRIDGE_DATATYPE_KEYCODE;
      value_ = (p->key).get();
      ic_.begin();
    }
  }
  {
    auto p = newval.get_Params_KeyboardSpecialEventCallback();

    if (p && p->ex_iskeydown) {
      datatype_ = BRIDGE_DATATYPE_CONSUMERKEYCODE;
      value_ = (p->key).get();
      ic_.begin();
    }
  }
  {
    auto p = newval.get_Params_RelativePointerEventCallback();

    if (p && p->ex_button != PointingButton::NONE && p->ex_isbuttondown) {
      datatype_ = BRIDGE_DATATYPE_POINTINGBUTTON;
      value_ = (p->ex_button).get();
      ic_.begin();
    }
  }
}
}
