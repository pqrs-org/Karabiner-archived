#ifndef FLIPSCROLLWHEEL_HPP
#define FLIPSCROLLWHEEL_HPP

#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class FlipScrollWheel {
    public:
      FlipScrollWheel(void);
      ~FlipScrollWheel(void);

      bool remap(RemapParams& remapParams);

      void add(AddDataType datatype, AddValue newval);

    private:
      bool flipHorizontalScroll_;
      bool flipVerticalScroll_;
    };
  }
}

#endif
