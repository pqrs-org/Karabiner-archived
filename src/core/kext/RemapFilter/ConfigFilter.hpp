#ifndef CONFIGFILTER_HPP
#define CONFIGFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class ConfigFilter : public RemapFilterBase {
    public:
      ConfigFilter(unsigned int type) : RemapFilterBase(type) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      Vector_FilterValue targets_;
    };
  }
}

#endif
