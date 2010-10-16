#ifndef FILTERUNION_HPP
#define FILTERUNION_HPP

#include "bridge.h"
#include "ApplicationFilter.hpp"
#include "ConfigFilter.hpp"
#include "DeviceFilter.hpp"
#include "InputModeFilter.hpp"

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

      unsigned int type_;

      union {
        ApplicationFilter* applicationFilter;
        ConfigFilter* configFilter;
        DeviceFilter* deviceFilter;
        InputModeFilter* inputModeFilter;
      } p_;
    };

    typedef FilterUnion* FilterUnionPointer;
    DECLARE_VECTOR(FilterUnionPointer);
  }
}

#endif
