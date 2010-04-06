#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "bridge.hpp"
#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class VirtualKey {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);
  };

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
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static bool handle(Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

  private:
    enum {
      KEYEVENT_DELAY_MS = 100,
    };
    static bool handle_core(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
                            KeyCode key,
                            const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& inputmodedetail);
    static bool handle_RESTORE(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    static void firekeytoinputdetail(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata,
                                     KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail inputmodedetail);

    static KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail normalize(const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& imd);

    static void fire(OSObject* notuse_owner, IOTimerEventSource* notuse_sender);
    static void fire_nolock(void);

    static KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail savedinputmodedetail_;
    static KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail currentinputmodedetail_;

    struct FireKeyInfo {
      Flags flags;
      KeyCode key;
      KeyboardType keyboardType;
      KeyRemap4MacBook_bridge::GetWorkspaceData::Reply workspacedata;
      bool active;
    };
    static FireKeyInfo fireKeyInfo_;
    static TimerWrapper timer_;
  };
}

#endif
