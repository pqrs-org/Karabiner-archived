#ifndef CONFIGFILTER_HPP
#define CONFIGFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class ConfigFilter {
    public:
      ConfigFilter(unsigned int t) : type_(t) {}
      ~ConfigFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue targets_;
    };
  }
}

#endif
