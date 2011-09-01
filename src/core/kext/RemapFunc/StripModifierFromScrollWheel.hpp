#ifndef STRIPMODIFIERFROMSCROLLWHEEL_HPP
#define STRIPMODIFIERFROMSCROLLWHEEL_HPP

#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class StripModifierFromScrollWheel {
    public:
      StripModifierFromScrollWheel(void);
      ~StripModifierFromScrollWheel(void);

      bool remap(RemapPointingParams_scroll& remapParams);

      void add(unsigned int datatype, unsigned int newval);

    private:
      Flags flags_;
    };
  }
}

#endif
