#ifndef DROPSCROLLWHEEL_HPP
#define DROPSCROLLWHEEL_HPP

#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class DropScrollWheel {
    public:
      DropScrollWheel(void);
      ~DropScrollWheel(void);

      bool remap(RemapParams& remapParams);

      void add(AddDataType datatype, AddValue newval);

    private:
      bool dropHorizontalScroll_;
    };
  }
}

#endif
