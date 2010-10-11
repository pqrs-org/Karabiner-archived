#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    struct FilterValue {
      FilterValue(void) : value(0) {}
      FilterValue(unsigned int v) : value(v) {}

      unsigned int value;
    };
    DECLARE_VECTOR(FilterValue);

    struct DeviceFilterValue {
      DeviceFilterValue(void) : vendorID(0), productID(0) {}
      DeviceFilterValue(unsigned int v, unsigned int p) : vendorID(v), productID(p) {}

      unsigned int vendorID;
      unsigned int productID;
    };
    DECLARE_VECTOR(DeviceFilterValue);

    struct ConfigPointer {
      ConfigPointer(void) : pointer(NULL) {}
      ConfigPointer(int* p) : pointer(p) {}

      int* pointer;
    };
    DECLARE_VECTOR(ConfigPointer);
  }
}

#endif
