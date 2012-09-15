#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "Vector.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    typedef unsigned int FilterValue;
    DECLARE_VECTOR(FilterValue);

    // For <device_only>, <device_not>
    DECLARE_VECTOR(DeviceIdentifier);
  }
}

#endif
