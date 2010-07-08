#ifndef REMAP_HPP
#define REMAP_HPP

#include "base.hpp"
#include "bridge.hpp"
#include "KeyCode.hpp"
#include "CallBackWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  struct RemapParams {
    RemapParams(const Params_KeyboardEventCallBack& p) :
      params(p), isremapped(false) {}

    const Params_KeyboardEventCallBack& params;
    bool isremapped;

    bool isKeyDownOrModifierDown(void) const { return params.eventType.isKeyDownOrModifierDown(params.key, params.flags); }
  };

  // --------------------
  struct RemapConsumerParams {
    RemapConsumerParams(const Params_KeyboardSpecialEventCallback& p) :
      params(p), isremapped(false) {}

    const Params_KeyboardSpecialEventCallback& params;
    bool isremapped;
  };

  // --------------------
  struct RemapPointingParams_relative {
    RemapPointingParams_relative(const Params_RelativePointerEventCallback& p) :
      params(p), isremapped(false) {}

    const Params_RelativePointerEventCallback& params;
    bool isremapped;
  };
}

#endif
