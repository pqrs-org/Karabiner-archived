#ifndef WINDOWNAMEFILTER_HPP
#define WINDOWNAMEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class WindowNameFilter {
    public:
      WindowNameFilter(unsigned int t) : type_(t) {}
      ~WindowNameFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue targets_;
    };
  }
}

#endif
