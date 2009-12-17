#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "bridge.hpp"
#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  class Handle_VK_LOCK_common {
  public:
    Handle_VK_LOCK_common(const KeyCode& k, const ModifierFlag& f) :
      active_(false), key_(k), flag_(f) {}
    bool handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

  private:
    bool active_;
    const KeyCode& key_;
    const ModifierFlag& flag_;
  };

  class Handle_VK_LOCK_FN {
  public:
    static bool handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) { return h_.handle(params, workspacedata); }
  private:
    static Handle_VK_LOCK_common h_;
  };

  class Handle_VK_LOCK_COMMAND_R {
  public:
    static bool handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) { return h_.handle(params, workspacedata); }
  private:
    static Handle_VK_LOCK_common h_;
  };
  // ----------------------------------------------------------------------
}

#endif
