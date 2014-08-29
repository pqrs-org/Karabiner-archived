#ifndef MODIFIERLOCKEDFILTER_HPP
#define MODIFIERLOCKEDFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class ModifierLockedFilter {
    public:
      ModifierLockedFilter(unsigned int t);
      ~ModifierLockedFilter(void) {}

      void initialize(const unsigned int* vec, size_t length);
      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_Vector_ModifierFlag targets_;
    };
  }
}

#endif
