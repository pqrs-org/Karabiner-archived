#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "bridge.hpp"
#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  class Handle_VK_LOCK {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_CHANGE_INPUTMODE {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_CONFIG {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);
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
      return handle_core(params, workspacedata,
                         KeyCode::VK_JIS_TEMPORARY_ROMAN,
                         KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN);
    }
    static bool handle_HIRAGANA(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle_core(params, workspacedata,
                         KeyCode::VK_JIS_TEMPORARY_HIRAGANA,
                         KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA);
    }
    static bool handle_KATAKANA(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle_core(params, workspacedata,
                         KeyCode::VK_JIS_TEMPORARY_KATAKANA,
                         KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_KATAKANA);
    }
    // OK, Ainu is not Japanese.
    // But the input source of Ainu is Kotoeri, we need to handle it here.
    static bool handle_AINU(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      return handle_core(params, workspacedata,
                         KeyCode::VK_JIS_TEMPORARY_AINU,
                         KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_AINU);
    }
    static bool handle_RESTORE(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    static bool handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      if (handle_ROMAN(params, workspacedata)) return true;
      if (handle_HIRAGANA(params, workspacedata)) return true;
      if (handle_KATAKANA(params, workspacedata)) return true;
      if (handle_AINU(params, workspacedata)) return true;
      if (handle_RESTORE(params, workspacedata)) return true;
      return false;
    }

  private:
    // We use this class instance as global variable.
    // The value of KeyCode (and ModifierFlag) are unsettled at the constructer,
    // so we need to use reference at arguments.
    static bool handle_core(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
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
