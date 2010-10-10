#ifndef INPUTMODEFILTER_HPP
#define INPUTMODEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class InputModeFilter {
    public:
      InputModeFilter(unsigned int t);
      ~InputModeFilter(void);

      void add(unsigned int newval);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue* targets_;
    };
  }
}

#endif
