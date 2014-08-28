#ifndef DEVICEFILTER_HPP
#define DEVICEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class DeviceFilter {
    public:
      DeviceFilter(unsigned int t) : type_(t) {};
      ~DeviceFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_DeviceIdentifier targets_;
    };
  }
}

#endif
