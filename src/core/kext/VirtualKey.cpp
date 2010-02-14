#include "base.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "VirtualKey.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  Flags Handle_VK_LOCK_common::statusMessageFlags(0);

  bool
  Handle_VK_LOCK_common::handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != key_) return false;

    if (params.eventType == EventType::DOWN && params.repeat == false) {
      active_ = ! active_;
      if (active_) {
        FlagStatus::lock_increase(flag_);
      } else {
        FlagStatus::lock_decrease(flag_);
      }
    }

    // ------------------------------------------------------------
    // handle StatusMessage
    if (! config.general_hide_modifier_lock_status) {
      Flags f = FlagStatus::getLockedFlags();
      if (f != statusMessageFlags) {
        KeyRemap4MacBook_bridge::StatusMessage::Request request(KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_MODIFIER_LOCK, "Lock: ");
        bool isempty = true;

        if (f.isOn(ModifierFlag::FN)) {
          isempty = false;
          strlcat(request.message, "FN ", sizeof(request.message));
        }
        if (f.isOn(ModifierFlag::COMMAND_L) || f.isOn(ModifierFlag::COMMAND_R)) {
          isempty = false;
          strlcat(request.message, "Cmd ", sizeof(request.message));
        }

        if (isempty) {
          request.message[0] = '\0';
        }
        KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE, &request, sizeof(request), NULL, 0);

        statusMessageFlags = f;
      }
    }

    // ------------------------------------------------------------
    return true;
  }

  Handle_VK_LOCK_common Handle_VK_LOCK_FN::h_(KeyCode::VK_LOCK_FN, ModifierFlag::FN);
  Handle_VK_LOCK_common Handle_VK_LOCK_COMMAND_R::h_(KeyCode::VK_LOCK_COMMAND_R, ModifierFlag::COMMAND_R);

  // ----------------------------------------------------------------------
  bool
  Handle_VK_CHANGE_INPUTMODE::handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    KeyRemap4MacBook_bridge::ChangeInputMode::Request request;
    if (params.key == KeyCode::VK_CHANGE_INPUTMODE_ENGLISH) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_ENGLISH;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_FRENCH) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_FRENCH;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_GERMAN) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_GERMAN;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_JAPANESE) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_JAPANESE;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_SWEDISH) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_SWEDISH;
    } else if (params.key == KeyCode::VK_CHANGE_INPUTMODE_CANADIAN) {
      request.inputmode = KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_CANADIAN;
    } else {
      return false;
    }

    if (params.eventType == EventType::DOWN && params.repeat == false) {
      KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_CHANGE_INPUTMODE, &request, sizeof(request), NULL, 0);
    }
    return true;
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_TOGGLE_CONFIG::handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    int *configitem = NULL;

    if (params.key == KeyCode::VK_TOGGLE_CONFIG_remap_completely_vimode) {
      configitem = &(config.remap_completely_vimode);
    } else {
      return false;
    }

    // ------------------------------------------------------------
    if (params.eventType == EventType::DOWN && params.repeat == false) {
      if (configitem) {
        *configitem = ! *configitem;
        refresh_remapfunc();
      }
    }
    return true;
  }

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
  handle_VK_JIS_EISUU_x2(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
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
  handle_VK_JIS_KANA_x2(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
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

  // ----------------------------------------------------------------------
  bool
  Handle_VK_JIS_TEMPORARY::handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
                                  KeyCode key,
                                  const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& inputmodedetail)
  {
    if (params.key != key) return false;

    if (params.eventType == EventType::DOWN) {
      if (savedinputmodedetail_ == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE) {
        savedinputmodedetail_ = workspacedata.inputmodedetail;
        currentinputmodedetail_ = workspacedata.inputmodedetail;
      }
      firekeytoinputdetail(params, workspacedata, inputmodedetail);
    }

    return true;
  }

  bool
  Handle_VK_JIS_TEMPORARY::handle_RESTORE(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
  {
    if (params.key != KeyCode::VK_JIS_TEMPORARY_RESTORE) return false;

    if (params.eventType == EventType::DOWN) {
      if (savedinputmodedetail_ != KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE) {
        firekeytoinputdetail(params, workspacedata, savedinputmodedetail_);
        savedinputmodedetail_ = KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;
        currentinputmodedetail_ = KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;
      }
    }

    return true;
  }

  void
  Handle_VK_JIS_TEMPORARY::firekeytoinputdetail(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
                                                KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail inputmodedetail)
  {
    inputmodedetail = normalize(inputmodedetail);
    currentinputmodedetail_ = normalize(currentinputmodedetail_);

    // ------------------------------------------------------------
    if (inputmodedetail == currentinputmodedetail_) return;
    if (inputmodedetail == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE) return;

    currentinputmodedetail_ = inputmodedetail;

    // ------------------------------------------------------------
    if (inputmodedetail == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN) {
      RemapUtil::fireKey_downup(Flags(0), KeyCode::JIS_EISUU, params.keyboardType, workspacedata);
      return;
    }

    if (inputmodedetail == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA) {
      RemapUtil::fireKey_downup(Flags(0), KeyCode::JIS_KANA, params.keyboardType, workspacedata);
      return;
    }

    if (inputmodedetail == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_KATAKANA) {
      RemapUtil::fireKey_downup(ModifierFlag::SHIFT_L, KeyCode::JIS_KANA, params.keyboardType, workspacedata);
      return;
    }
  }

  KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail
  Handle_VK_JIS_TEMPORARY::normalize(const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& imd)
  {
    if (imd == KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE) {
      return KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA;
    }
    return imd;
  }

  KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail Handle_VK_JIS_TEMPORARY::savedinputmodedetail_ = KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;
  KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail Handle_VK_JIS_TEMPORARY::currentinputmodedetail_ = KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;
}
