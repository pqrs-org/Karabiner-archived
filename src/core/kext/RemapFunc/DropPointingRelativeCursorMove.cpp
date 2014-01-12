#include <IOKit/IOLib.h>

#include "DropPointingRelativeCursorMove.hpp"
#include "IOLogWrapper.hpp"

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
    DropPointingRelativeCursorMove::remap(RemapParams& remapParams)
    {
      Params_RelativePointerEventCallback* params = remapParams.paramsUnion.get_Params_RelativePointerEventCallback();
      if (! params) return false;

      if (remapParams.isremapped) return false;

      if (params->dx != 0 || params->dy != 0) {
        remapParams.isremapped = true;
        return true;
      }

      return false;
    }
  }
}
