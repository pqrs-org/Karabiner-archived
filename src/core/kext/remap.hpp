#ifndef REMAP_HPP
#define REMAP_HPP

#include "base.hpp"
#include "bridge.hpp"
#include "KeyCode.hpp"
#include "CallBackWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  struct RemapParams {
    RemapParams(const Params_KeyboardEventCallBack& p, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& w) :
      params(p), workspacedata(w), isremapped(false) {}

    const Params_KeyboardEventCallBack& params;
    const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata;
    bool isremapped;

    void drop(void) { isremapped = true; }
    bool isKeyDownOrModifierDown(void) const { return params.eventType.isKeyDownOrModifierDown(params.key, params.flags); }
  };
  void remap_core(RemapParams& remapParams);

  // --------------------
  struct RemapConsumerParams {
    Params_KeyboardSpecialEventCallback& params;
    KeyRemap4MacBook_bridge::GetWorkspaceData::Reply workspacedata;
    bool& isremapped;

    void drop(void) const { isremapped = true; }
  };
  void remap_consumer(const RemapConsumerParams& remapParams);

  // --------------------
  struct RemapPointingParams_relative {
    RemapPointingParams_relative(const Params_RelativePointerEventCallback& p) :
      params(p), isremapped(false) {}

    const Params_RelativePointerEventCallback& params;
    bool isremapped;

    void drop(void) { isremapped = true; }
  };
  void remap_pointing_relative_core(RemapPointingParams_relative& remapParams);
}

#endif
