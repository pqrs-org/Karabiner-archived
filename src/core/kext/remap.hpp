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
    RemapPointingParams_relative(const ParamsUnion& p) :
      paramsUnion(p), params(*(p.params.params_RelativePointerEventCallback)), isremapped(false) {}

    const ParamsUnion& paramsUnion;
    // XXX: Please remove params.
    const Params_RelativePointerEventCallback& params;
    bool isremapped;
  };

  // --------------------
  struct RemapPointingParams_scroll {
    RemapPointingParams_scroll(const ParamsUnion& p) :
      paramsUnion(p), params(*(p.params.params_ScrollWheelEventCallback)), isremapped(false) {}

    const ParamsUnion& paramsUnion;
    // XXX: Please remove params.
    const Params_ScrollWheelEventCallback& params;
    bool isremapped;
  };
}

#endif
