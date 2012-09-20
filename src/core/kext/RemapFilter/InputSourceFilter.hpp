#ifndef INPUTSOURCEFILTER_HPP
#define INPUTSOURCEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class InputSourceFilter {
    public:
      InputSourceFilter(unsigned int t);
      ~InputSourceFilter(void);

      void add(unsigned int newval);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue* targets_;
    };
  }
}

#endif
