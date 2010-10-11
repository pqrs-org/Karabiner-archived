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
      FilterUnion(void) {
        type_ = BRIDGE_FILTERTYPE_NONE;
      }
      void initialize(const unsigned int* vec, size_t length);
      void terminate(void);

      bool isblocked(void);

      unsigned int type_;

      union {
        ApplicationFilter* applicationFilter;
        ConfigFilter* configFilter;
        DeviceFilter* deviceFilter;
        InputModeFilter* inputModeFilter;
      } p_;
    };
  }
}

#endif
