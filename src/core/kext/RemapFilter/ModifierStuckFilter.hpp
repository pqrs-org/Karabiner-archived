#ifndef MODIFIERSTUCKFILTER_HPP
#define MODIFIERSTUCKFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class ModifierStuckFilter {
    public:
      ModifierStuckFilter(unsigned int t);
      ~ModifierStuckFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_Vector_ModifierFlag targets_;
    };
  }
}

#endif
