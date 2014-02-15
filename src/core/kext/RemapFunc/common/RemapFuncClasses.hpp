#ifndef REMAPFUNCCLASSES_HPP
#define REMAPFUNCCLASSES_HPP

#include "FlagStatus.hpp"
#include "KeyCode.hpp"
#include "ParamsUnion.hpp"
#include "Vector.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  struct RemapParams {
    RemapParams(const ParamsUnion& p) :
      paramsUnion(p), isremapped(false) {}

    const ParamsUnion& paramsUnion;
    bool isremapped;
  };

  namespace RemapFunc {
    struct PairKeyFlags {
      PairKeyFlags(void) {}
      PairKeyFlags(KeyCode k) : key(k) {}

      KeyCode key;
      Flags flags;
    };
    DECLARE_VECTOR(PairKeyFlags);
  }
}

#endif
