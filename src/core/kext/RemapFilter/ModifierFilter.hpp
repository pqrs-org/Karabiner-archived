#ifndef MODIFIERFILTER_HPP
#define MODIFIERFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class ModifierFilter {
    public:
      ModifierFilter(unsigned int t);
      ~ModifierFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_Vector_ModifierFlag targets_;
    };
  }
}

#endif
