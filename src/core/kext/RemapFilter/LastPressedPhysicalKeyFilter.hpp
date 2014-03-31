#ifndef LASTPRESSEDPHYSICALKEYFILTER_HPP
#define LASTPRESSEDPHYSICALKEYFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class LastPressedPhysicalKeyFilter {
    public:
      LastPressedPhysicalKeyFilter(unsigned int t);
      ~LastPressedPhysicalKeyFilter(void);

      void add(AddDataType datatype, AddValue value);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValueWithDataType targets_;
    };
  }
}

#endif
