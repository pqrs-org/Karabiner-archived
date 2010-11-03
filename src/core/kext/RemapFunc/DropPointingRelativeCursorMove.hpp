#ifndef DROPPOINTINGRELATIVECURSORMOVE_HPP
#define DROPPOINTINGRELATIVECURSORMOVE_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class DropPointingRelativeCursorMove {
    public:
      DropPointingRelativeCursorMove(void);
      ~DropPointingRelativeCursorMove(void);

      bool remap(RemapPointingParams_relative& remapParams);

      void add(unsigned int datatype, unsigned int newval);
    };
  }
}

#endif
