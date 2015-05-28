#ifndef REMAPFUNCCLASSES_HPP
#define REMAPFUNCCLASSES_HPP

#include "FlagStatus.hpp"
#include "KeyCode.hpp"
#include "Params.hpp"
#include "Vector.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
struct RemapParams {
  RemapParams(const Params_Base& p, PhysicalEventType pet) : paramsBase(p),
                                                             physicalEventType(pet),
                                                             isremapped(false) {}

  const Params_Base& paramsBase;
  const PhysicalEventType physicalEventType;
  bool isremapped;
};
}

#endif
