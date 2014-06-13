#ifndef DROPPOINTINGRELATIVECURSORMOVE_HPP
#define DROPPOINTINGRELATIVECURSORMOVE_HPP

#include "RemapFuncClasses.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class DropPointingRelativeCursorMove {
    public:
      DropPointingRelativeCursorMove(void);
      ~DropPointingRelativeCursorMove(void);

      bool remap(RemapParams& remapParams);

      void add(AddDataType datatype, AddValue newval);
    };
  }
}

#endif
