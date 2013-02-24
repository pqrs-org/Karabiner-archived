#ifndef FLIPSCROLLWHEEL_HPP
#define FLIPSCROLLWHEEL_HPP

#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class FlipScrollWheel {
    public:
      FlipScrollWheel(void);
      ~FlipScrollWheel(void);

      bool remap(RemapPointingParams_scroll& remapParams);

      void add(unsigned int datatype, unsigned int newval);

    private:
      bool flipHorizontalScroll_;
      bool flipVerticalScroll_;
    };
  }
}

#endif
