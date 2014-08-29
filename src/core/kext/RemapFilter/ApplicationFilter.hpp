#ifndef APPLICATIONFILTER_HPP
#define APPLICATIONFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class ApplicationFilter : public RemapFilterBase {
    public:
      ApplicationFilter(unsigned int type) : RemapFilterBase(type) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      Vector_FilterValue targets_;
    };
  }
}

#endif
