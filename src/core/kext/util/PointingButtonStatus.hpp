#ifndef POINTINGBUTTONSTATUS_HPP
#define POINTINGBUTTONSTATUS_HPP

#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace PointingButtonStatus {
    void set(PointingButton::PointingButton button, bool isdown);
    unsigned int get(void);
  };
}

#endif
