#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "bridge.h"
#include "Vector.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class RemapFilterBase {
    public:
      RemapFilterBase(unsigned int type) : type_(type) {}
      virtual ~RemapFilterBase(void) {}

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

      virtual void initialize(const unsigned int* vec, size_t length) = 0;
      virtual bool isblocked(void) = 0;

      unsigned int get_type(void) const { return type_; }

    private:
      unsigned int type_;
    };

    typedef RemapFilterBase* RemapFilterBasePointer;
    DECLARE_VECTOR(RemapFilterBasePointer);
  }
}

#endif
