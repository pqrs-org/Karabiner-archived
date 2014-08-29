#ifndef INPUTSOURCEFILTER_HPP
#define INPUTSOURCEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class InputSourceFilter {
    public:
      InputSourceFilter(unsigned int t) : type_(t) {}
      ~InputSourceFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_FilterValue targets_;
    };
  }
}

#endif
