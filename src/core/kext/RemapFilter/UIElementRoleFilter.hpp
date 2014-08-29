#ifndef UIELEMENTROLEFILTER_HPP
#define UIELEMENTROLEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class UIElementRoleFilter {
    public:
      UIElementRoleFilter(unsigned int t) : type_(t) {}
      ~UIElementRoleFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue targets_;
    };
  }
}

#endif
