#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    typedef unsigned int FilterValue;
    DECLARE_VECTOR(FilterValue);

    struct DeviceFilterValue {
      DeviceFilterValue(void) : vendorID(0), productID(0) {}
      DeviceFilterValue(unsigned int v, unsigned int p) : vendorID(v), productID(p) {}

      unsigned int vendorID;
      unsigned int productID;
    };
    DECLARE_VECTOR(DeviceFilterValue);

    typedef int* ConfigPointer;
    DECLARE_VECTOR(ConfigPointer);
  }
}

#endif
