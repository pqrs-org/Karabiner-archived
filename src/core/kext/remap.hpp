#ifndef REMAP_HPP
#define REMAP_HPP

#include "CallBackWrapper.hpp"
#include "KeyCode.hpp"
#include "ParamsUnion.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  struct RemapParams {
    RemapParams(const ParamsUnion& p) :
      paramsUnion(p), params(*(p.params.params_KeyboardEventCallBack)), isremapped(false) {}

    const ParamsUnion& paramsUnion;
    // XXX: Please remove params.
    const Params_KeyboardEventCallBack& params;
    bool isremapped;
  };

  // --------------------
  struct RemapConsumerParams {
    RemapConsumerParams(const ParamsUnion& p) :
      paramsUnion(p), params(*(p.params.params_KeyboardSpecialEventCallback)), isremapped(false) {}

    const ParamsUnion& paramsUnion;
    // XXX: Please remove params.
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

  // --------------------
  struct RemapPointingParams_scroll {
    RemapPointingParams_scroll(const Params_ScrollWheelEventCallback& p) :
      params(p), isremapped(false) {}

    const Params_ScrollWheelEventCallback& params;
    bool isremapped;
  };
}

#endif
