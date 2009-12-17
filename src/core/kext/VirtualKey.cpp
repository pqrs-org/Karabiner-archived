#include "FlagStatus.hpp"
#include "VirtualKey.hpp"
#include "RemapUtil.hpp"

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
  bool
  Handle_VK_JIS_TOGGLE_EISUU_KANA::handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

    if (params.eventType == EventType::DOWN) {
      if (workspacedata.inputmode == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE) {
        newkeycode_ = KeyCode::JIS_EISUU;
      } else {
        newkeycode_ = KeyCode::JIS_KANA;
      }
    }

    params.key = newkeycode_;
    RemapUtil::fireKey(params, workspacedata);
    return true;
  }

  KeyCode Handle_VK_JIS_TOGGLE_EISUU_KANA::newkeycode_;

  // ----------------------------------------------------------------------
  bool
  handle_VK_JIS_EISUU_x2(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_EISUU_x2) return false;

    if (params.eventType == EventType::DOWN) {
      for (int i = 0; i < 2; ++i) {
        RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_EISUU, params.keyboardType, workspacedata);
      }
    }
    return true;
  }

  bool
  handle_VK_JIS_KANA_x2(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_KANA_x2) return false;

    if (params.eventType == EventType::DOWN) {
      for (int i = 0; i < 2; ++i) {
        RemapUtil::fireKey_downup(params.flags, KeyCode::JIS_KANA, params.keyboardType, workspacedata);
      }
    }
    return true;
  }

  bool
  handle_VK_JIS_BACKSLASH(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_BACKSLASH) return false;

    params.key = KeyCode::BACKSLASH;
    params.keyboardType = KeyboardType::MACBOOK;
    RemapUtil::fireKey(params, workspacedata);
    return true;
  }
}
