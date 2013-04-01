#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "bridge.h"
#include "Vector.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    typedef unsigned int FilterValue;
    DECLARE_VECTOR(FilterValue);

    class FilterValueWithDataType {
    public:
      FilterValueWithDataType(void) : datatype(BRIDGE_DATATYPE_NONE), value(0) {}
      FilterValueWithDataType(unsigned int d, unsigned int v) : datatype(d), value(v) {}
      unsigned int datatype;
      unsigned int value;
    };
    DECLARE_VECTOR(FilterValueWithDataType);

    // For <device_only>, <device_not>
    DECLARE_VECTOR(DeviceIdentifier);
  }
}

#endif
