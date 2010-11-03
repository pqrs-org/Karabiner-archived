#include "DropPointingRelativeCursorMove.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    DropPointingRelativeCursorMove::DropPointingRelativeCursorMove(void)
    {}

    DropPointingRelativeCursorMove::~DropPointingRelativeCursorMove(void)
    {}

    void
    DropPointingRelativeCursorMove::add(unsigned int datatype, unsigned int newval)
    {}

    bool
    DropPointingRelativeCursorMove::remap(RemapPointingParams_relative& remapParams)
    {
      if (remapParams.isremapped) return false;

      if (remapParams.params.dx != 0 || remapParams.params.dy != 0) {
        remapParams.isremapped = true;
        return true;
      }

      return false;
    }
  }
}
