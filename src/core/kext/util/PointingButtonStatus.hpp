#ifndef POINTINGBUTTONSTATUS_HPP
#define POINTINGBUTTONSTATUS_HPP

#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace PointingButtonStatus {
    void set(const PointingButton& button, bool isdown);
    Buttons get(void);
  };
}

#endif
