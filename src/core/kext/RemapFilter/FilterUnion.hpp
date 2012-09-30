#ifndef FILTERUNION_HPP
#define FILTERUNION_HPP

#include "bridge.h"
#include "ApplicationFilter.hpp"
#include "ConfigFilter.hpp"
#include "ModifierFilter.hpp"
#include "DeviceFilter.hpp"
#include "InputSourceFilter.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class FilterUnion {
    public:
      FilterUnion(const unsigned int* vec, size_t length);
      ~FilterUnion(void);

      bool isblocked(void);

    private:
      void initialize(const unsigned int* vec, size_t length);
      void terminate(void);

      FilterUnion(const FilterUnion& rhs);
      FilterUnion& operator=(const FilterUnion& rhs);

      unsigned int type_;

      union {
        ApplicationFilter* applicationFilter;
        ConfigFilter* configFilter;
        ModifierFilter* modifierFilter;
        DeviceFilter* deviceFilter;
        InputSourceFilter* inputSourceFilter;
      } p_;
    };

    typedef FilterUnion* FilterUnionPointer;
    DECLARE_VECTOR(FilterUnionPointer);
  }
}

#endif
