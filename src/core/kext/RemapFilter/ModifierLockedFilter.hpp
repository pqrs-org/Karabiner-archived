#ifndef MODIFIERLOCKEDFILTER_HPP
#define MODIFIERLOCKEDFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class ModifierLockedFilter {
    public:
      ModifierLockedFilter(unsigned int t);
      ~ModifierLockedFilter(void);

      void add(AddDataType datatype, AddValue newval);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_Vector_ModifierFlag targets_;
    };
  }
}

#endif
