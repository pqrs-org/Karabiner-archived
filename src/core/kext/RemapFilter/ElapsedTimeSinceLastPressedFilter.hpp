#ifndef ELAPSEDTIMESINCELASTPRESSEDFILTER_HPP
#define ELAPSEDTIMESINCELASTPRESSEDFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class ElapsedTimeSinceLastPressedFilter {
    public:
      ElapsedTimeSinceLastPressedFilter(unsigned int t) : type_(t) {}
      ~ElapsedTimeSinceLastPressedFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValueWithDataType targets_;
    };
  }
}

#endif
