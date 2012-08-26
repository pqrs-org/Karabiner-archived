#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    typedef unsigned int FilterValue;
    DECLARE_VECTOR(FilterValue);

    struct DeviceFilterValue {
      DeviceFilterValue(void) :
        vendorID(0),
        productID(0),
        locationID(0)
      {}
      DeviceFilterValue(unsigned int v, unsigned int p, unsigned int l) :
        vendorID(v),
        productID(p),
        locationID(l)
      {}

      unsigned int vendorID;
      unsigned int productID;
      unsigned int locationID;
    };
    DECLARE_VECTOR(DeviceFilterValue);
  }
}

#endif
