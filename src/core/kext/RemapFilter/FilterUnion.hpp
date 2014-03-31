#ifndef FILTERUNION_HPP
#define FILTERUNION_HPP

#include "bridge.h"
#include "ApplicationFilter.hpp"
#include "ConfigFilter.hpp"
#include "DeviceFilter.hpp"
#include "ElapsedTimeSinceLastPressedFilter.hpp"
#include "InputSourceFilter.hpp"
#include "LastPressedPhysicalKeyFilter.hpp"
#include "ModifierFilter.hpp"

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

      FilterUnion(const FilterUnion& rhs); // Prevent copy-construction
      FilterUnion& operator=(const FilterUnion& rhs); // Prevent assignment

      unsigned int type_;

      union {
        ApplicationFilter* applicationFilter;
        ConfigFilter* configFilter;
        DeviceFilter* deviceFilter;
        ElapsedTimeSinceLastPressedFilter* elapsedTimeSinceLastPressedFilter;
        InputSourceFilter* inputSourceFilter;
        LastPressedPhysicalKeyFilter* lastPressedPhysicalKeyFilter;
        ModifierFilter* modifierFilter;
      } p_;
    };

    typedef FilterUnion* FilterUnionPointer;
    DECLARE_VECTOR(FilterUnionPointer);
  }
}

#endif
