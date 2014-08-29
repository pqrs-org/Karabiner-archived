#ifndef LASTPRESSEDPHYSICALKEYFILTER_HPP
#define LASTPRESSEDPHYSICALKEYFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class LastPressedPhysicalKeyFilter {
    public:
      LastPressedPhysicalKeyFilter(unsigned int t) : type_(t) {}
      ~LastPressedPhysicalKeyFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValueWithDataType targets_;
    };
  }
}

#endif
