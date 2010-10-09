#ifndef APPLICATIONFILTER_HPP
#define APPLICATIONFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class ApplicationFilter {
    public:
      ApplicationFilter(unsigned int t);
      ~ApplicationFilter(void);

      void add(unsigned int newval);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue* targets_;
    };
  }
}

#endif
