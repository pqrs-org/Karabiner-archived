#include "PointingButtonStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace PointingButtonStatus {
    bool helddown_left = false;
    bool helddown_right = false;
    bool helddown_middle = false;

    void
    set(PointingButton::PointingButton button, bool isdown)
    {
      if (button == PointingButton::LEFT) helddown_left = isdown;
      if (button == PointingButton::RIGHT) helddown_right = isdown;
      if (button == PointingButton::MIDDLE) helddown_middle = isdown;
    }

    unsigned int
    get(void)
    {
      unsigned int buttons = 0;
      if (helddown_left) buttons |= PointingButton::LEFT;
      if (helddown_right) buttons |= PointingButton::RIGHT;
      if (helddown_middle) buttons |= PointingButton::MIDDLE;
      return buttons;
    }
  }
}
