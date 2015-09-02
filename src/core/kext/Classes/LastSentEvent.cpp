#include "LastSentEvent.hpp"
#include "Params.hpp"

namespace org_pqrs_Karabiner {
void LastSentEvent::update(const Params_Base& newval) {
  datatype_ = BRIDGE_DATATYPE_NONE;
  eventtype_ = EventType::NONE;

  // ----------------------------------------
  {
    auto p = newval.get_Params_KeyboardEventCallBack();

    if (p) {
      datatype_ = BRIDGE_DATATYPE_KEYCODE;
      value_ = (p->key).get();
      eventtype_ = p->eventType;
      ic_.begin();
    }
  }
  {
    auto p = newval.get_Params_KeyboardSpecialEventCallback();

    if (p) {
      datatype_ = BRIDGE_DATATYPE_CONSUMERKEYCODE;
      value_ = (p->key).get();
      eventtype_ = p->eventType;
      ic_.begin();
    }
  }
  {
    auto p = newval.get_Params_RelativePointerEventCallback();

    if (p) {
      datatype_ = BRIDGE_DATATYPE_POINTINGBUTTON;
      value_ = (p->ex_button).get();

      bool keydown = false;
      if (newval.iskeydown(keydown)) {
        eventtype_ = keydown ? EventType::DOWN : EventType::UP;
      } else {
        eventtype_ = EventType::NONE;
      }

      ic_.begin();
    }
  }
  {
    auto p = newval.get_Params_ScrollWheelEventCallback();

    if (p) {
      datatype_ = BRIDGE_DATATYPE_SCROLLWHEEL;
      value_ = 0;
      eventtype_ = EventType::NONE;
      ic_.begin();
    }
  }
}
}
