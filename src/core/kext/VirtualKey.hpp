#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "bridge.hpp"
#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"
#include "TimerWrapper.hpp"
#include "Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class VirtualKey {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);
    static void reset(void);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_LOCK {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_STICKY {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_LAZY {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_CHANGE_INPUTMODE {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_CONFIG {
  public:
    static void initialize(void);
    static void terminate(void);

    static void add_item(unsigned int configindex,
                         unsigned int keycode_toggle,
                         unsigned int keycode_force_on,
                         unsigned int keycode_force_off,
                         unsigned int keycode_sync_keydownup);

    static bool handle(const Params_KeyboardEventCallBack& params);

  private:
    struct Item {
      Item(void) {};
      Item(unsigned int ci, unsigned int kc_toggle, unsigned int kc_force_on, unsigned int kc_force_off, unsigned int kc_sync) :
        configindex(ci), keycode_toggle(kc_toggle), keycode_force_on(kc_force_on), keycode_force_off(kc_force_off), keycode_sync_keydownup(kc_sync) {}
      unsigned int configindex;
      unsigned int keycode_toggle;
      unsigned int keycode_force_on;
      unsigned int keycode_force_off;
      unsigned int keycode_sync_keydownup;
    };
    DECLARE_VECTOR(Item);
    static Vector_Item* items_;
  };

  // ----------------------------------------------------------------------
  class Handle_VK_MOUSEKEY {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);
    static void reset(void);

    static bool handle(const Params_KeyboardEventCallBack& params);

  private:
    enum {
      TIMER_INTERVAL = 20,
      SCALE_MAX = 20,
      HIGHSPEED_RELATIVE_SCALE = 50,
      HIGHSPEED_SCROLL_SCALE = 50,
    };
    static void fire(OSObject* notuse_owner, IOTimerEventSource* notuse_sender);

    static int dx_;
    static int dy_;
    static int scale_;
    static bool highspeed_;
    static bool scrollmode_;
    static TimerWrapper timer_;
  };

  // ----------------------------------------------------------------------
  class Handle_VK_JIS_TOGGLE_EISUU_KANA {
  public:
    static bool handle(const Params_KeyboardEventCallBack& params);

  private:
    // It is necessary to save toKeyCode for KeyUp.
    static KeyCode newkeycode_;
  };

  bool handle_VK_JIS_BACKSLASH(const Params_KeyboardEventCallBack& params);
  bool handle_VK_JIS_YEN(const Params_KeyboardEventCallBack& params);

  // ----------------------------------------------------------------------
  class Handle_VK_JIS_TEMPORARY {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static bool handle(const Params_KeyboardEventCallBack& params);

  private:
    enum {
      KEYEVENT_DELAY_MS = 100,
    };
    static bool handle_core(const Params_KeyboardEventCallBack& params,
                            KeyCode key,
                            const KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail& inputmodedetail);
    static bool handle_RESTORE(const Params_KeyboardEventCallBack& params);

    static void firekeytoinputdetail(const Params_KeyboardEventCallBack& params,
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
      bool active;
    };
    static FireKeyInfo fireKeyInfo_;
    static TimerWrapper timer_;
  };
}

#endif
