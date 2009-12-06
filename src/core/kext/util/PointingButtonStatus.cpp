#include "PointingButtonStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace PointingButtonStatus {
    bool helddown_left = false;
    bool helddown_right = false;
    bool helddown_middle = false;

    void
    set(const PointingButton& button, bool isdown)
    {
      if (button == PointingButton::LEFT) helddown_left = isdown;
      if (button == PointingButton::RIGHT) helddown_right = isdown;
      if (button == PointingButton::MIDDLE) helddown_middle = isdown;
    }

    Buttons
    get(void)
    {
      Buttons buttons = 0;
      if (helddown_left) buttons.add(PointingButton::LEFT);
      if (helddown_right) buttons.add(PointingButton::RIGHT);
      if (helddown_middle) buttons.add(PointingButton::MIDDLE);
      return buttons;
    }
  }
}
