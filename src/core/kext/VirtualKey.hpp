#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "bridge.hpp"
#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  class Handle_VK_LOCK_common {
  public:
    // We use this class instance as global variable.
    // The value of KeyCode (and ModifierFlag) are unsettled at the constructer,
    // so we need to use reference at arguments.
    Handle_VK_LOCK_common(const KeyCode& k, const ModifierFlag& f) :
      active_(false), key_(k), flag_(f) {}
    bool handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

  private:
    bool active_;
    const KeyCode& key_;
    const ModifierFlag& flag_;
  };

  class Handle_VK_LOCK_FN {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) { return h_.handle(params, workspacedata); }
  private:
    static Handle_VK_LOCK_common h_;
  };

  class Handle_VK_LOCK_COMMAND_R {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) { return h_.handle(params, workspacedata); }
  private:
    static Handle_VK_LOCK_common h_;
  };

  // ----------------------------------------------------------------------
  class Handle_VK_CHANGE_INPUTMODE {
  public:
    static bool handle_ENGLISH(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle(params, KeyCode::VK_CHANGE_INPUTMODE_ENGLISH);
    }
    static bool handle_FRENCH(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle(params, KeyCode::VK_CHANGE_INPUTMODE_FRENCH);
    }
    static bool handle_GERMAN(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle(params, KeyCode::VK_CHANGE_INPUTMODE_GERMAN);
    }
    static bool handle_JAPANESE(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle(params, KeyCode::VK_CHANGE_INPUTMODE_JAPANESE);
    }

  private:
    static bool handle(const Params_KeyboardEventCallBack& params, KeyCode key);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_JIS_TOGGLE_EISUU_KANA {
  public:
    static bool handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

  private:
    // It is necessary to save toKeyCode for KeyUp.
    static KeyCode newkeycode_;
  };

  bool handle_VK_JIS_EISUU_x2(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);
  bool handle_VK_JIS_KANA_x2(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);
  bool handle_VK_JIS_BACKSLASH(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

  // ----------------------------------------------------------------------
  class Handle_VK_JIS_TEMPORARY {
  public:
    static bool handle_ROMAN(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle(params, workspacedata,
                    KeyCode::VK_JIS_TEMPORARY_ROMAN,
                    KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN);
    }
    static bool handle_HIRAGANA(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle(params, workspacedata,
                    KeyCode::VK_JIS_TEMPORARY_HIRAGANA,
                    KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA);
    }
    static bool handle_KATAKANA(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle(params, workspacedata,
                    KeyCode::VK_JIS_TEMPORARY_KATAKANA,
                    KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_KATAKANA);
    }
    static bool handle_RESTORE(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

  private:
    // We use this class instance as global variable.
    // The value of KeyCode (and ModifierFlag) are unsettled at the constructer,
    // so we need to use reference at arguments.
    static bool handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
                       KeyCode key,
                       const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& inputmodedetail);

    static void firekeytoinputdetail(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
                                     KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail inputmodedetail);

    static KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail normalize(const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& imd);

    static KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail savedinputmodedetail_;
    static KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail currentinputmodedetail_;
  };
}

#endif
