#ifndef WINDOWNAMEFILTER_HPP
#define WINDOWNAMEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class WindowNameFilter : public RemapFilterBase {
    public:
      WindowNameFilter(unsigned int type) : RemapFilterBase(type) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      Vector_FilterValue targets_;
    };
  }
}

#endif
