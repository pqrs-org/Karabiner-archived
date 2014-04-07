#ifndef MODIFYFILTER_HPP
#define MODIFYFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    class ModifierFilter {
    public:
      ModifierFilter(unsigned int t);
      ~ModifierFilter(void);

      void add(AddDataType datatype, AddValue newval);

      bool isblocked(void);

    private:
      unsigned int type_;
      Vector_Vector_ModifierFlag targets_;
    };
  }
}

#endif
