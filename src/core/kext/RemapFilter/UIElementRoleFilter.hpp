#ifndef UIELEMENTROLEFILTER_HPP
#define UIELEMENTROLEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class UIElementRoleFilter {
    public:
      UIElementRoleFilter(unsigned int t);
      ~UIElementRoleFilter(void);

      void add(AddValue newval);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue targets_;
    };
  }
}

#endif
