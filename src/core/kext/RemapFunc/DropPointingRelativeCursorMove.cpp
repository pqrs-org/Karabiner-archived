#include <IOKit/IOLib.h>

#include "DropPointingRelativeCursorMove.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    bool
    DropPointingRelativeCursorMove::remap(RemapParams& remapParams)
    {
      auto params = remapParams.paramsBase.get_Params_RelativePointerEventCallback();
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
