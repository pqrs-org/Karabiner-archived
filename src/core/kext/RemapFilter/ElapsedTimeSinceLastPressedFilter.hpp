#ifndef ELAPSEDTIMESINCELASTPRESSEDFILTER_HPP
#define ELAPSEDTIMESINCELASTPRESSEDFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class ElapsedTimeSinceLastPressedFilter {
    public:
      ElapsedTimeSinceLastPressedFilter(unsigned int t);
      ~ElapsedTimeSinceLastPressedFilter(void);

      void add(AddDataType datatype, AddValue value);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValueWithDataType targets_;
    };
  }
}

#endif
