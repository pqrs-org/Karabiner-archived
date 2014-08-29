#ifndef INPUTSOURCEFILTER_HPP
#define INPUTSOURCEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class InputSourceFilter : public RemapFilterBase {
    public:
      InputSourceFilter(unsigned int type) : RemapFilterBase(type) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      Vector_FilterValue targets_;
    };
  }
}

#endif
