#ifndef FLIPPOINTINGRELATIVE_HPP
#define FLIPPOINTINGRELATIVE_HPP

#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class FlipPointingRelative {
    public:
      FlipPointingRelative(void);
      ~FlipPointingRelative(void);

      bool remap(RemapParams& remapParams);

      void add(AddDataType datatype, AddValue newval);

    private:
      bool flipHorizontal_;
      bool flipVertical_;
    };
  }
}

#endif
