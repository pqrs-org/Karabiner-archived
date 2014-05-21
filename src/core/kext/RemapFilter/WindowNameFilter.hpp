#ifndef WINDOWNAMEFILTER_HPP
#define WINDOWNAMEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class WindowNameFilter {
    public:
      WindowNameFilter(unsigned int t);
      ~WindowNameFilter(void);

      void add(AddValue newval);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue targets_;
    };
  }
}

#endif
