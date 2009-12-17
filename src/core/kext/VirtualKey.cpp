#include "FlagStatus.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  bool
  Handle_VK_LOCK_common::handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != key_) return false;

    if (params.eventType == EventType::DOWN) {
      active_ = ! active_;
      if (active_) {
        FlagStatus::lock_increase(flag_);
      } else {
        FlagStatus::lock_decrease(flag_);
      }
    }

    return true;
  }

  Handle_VK_LOCK_common Handle_VK_LOCK_FN::h_(KeyCode::VK_LOCK_FN, ModifierFlag::FN);
  Handle_VK_LOCK_common Handle_VK_LOCK_COMMAND_R::h_(KeyCode::VK_LOCK_COMMAND_R, ModifierFlag::COMMAND_R);
  // ----------------------------------------------------------------------
}
