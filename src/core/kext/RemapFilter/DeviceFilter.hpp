#ifndef DEVICEFILTER_HPP
#define DEVICEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class DeviceFilter : public RemapFilterBase {
    public:
      DeviceFilter(unsigned int type) : RemapFilterBase(type) {};

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      Vector_DeviceIdentifier targets_;
    };
  }
}

#endif
