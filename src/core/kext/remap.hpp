#ifndef REMAP_HPP
#define REMAP_HPP

#include "CallBackWrapper.hpp"
#include "KeyCode.hpp"
#include "ParamsUnion.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  struct RemapParams {
    RemapParams(const ParamsUnion& p) :
      paramsUnion(p), isremapped(false) {}

    const ParamsUnion& paramsUnion;
    bool isremapped;
  };
}

#endif
