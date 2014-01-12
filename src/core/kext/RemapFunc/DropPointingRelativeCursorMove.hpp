#ifndef DROPPOINTINGRELATIVECURSORMOVE_HPP
#define DROPPOINTINGRELATIVECURSORMOVE_HPP

#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class DropPointingRelativeCursorMove {
    public:
      DropPointingRelativeCursorMove(void);
      ~DropPointingRelativeCursorMove(void);

      bool remap(RemapParams& remapParams);

      void add(unsigned int datatype, unsigned int newval);
    };
  }
}

#endif
