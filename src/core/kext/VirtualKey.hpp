#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"
#include "RemapClass.hpp"
#include "TimerWrapper.hpp"
#include "Vector.hpp"
#include "VirtualKey/VK_LOCK.hpp"
#include "VirtualKey/VK_STICKY.hpp"
#include "VirtualKey/VK_LAZY.hpp"
#include "VirtualKey/VK_CHANGE_INPUTMODE.hpp"
#include "VirtualKey/VK_CONFIG.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    void initialize(IOWorkLoop& workloop);
    void terminate(void);
    void reset(void);

    bool isKeyLikeModifier(KeyCode keycode);
  };

  // ----------------------------------------------------------------------
  class Handle_VK_MOUSEKEY {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);
    static void reset(void);

    static bool handle(const Params_KeyboardEventCallBack& params);
    static bool is_VK_MOUSEKEY(KeyCode keycode);

  private:
    enum {
      TIMER_INTERVAL = 20,
      SCALE_MAX = 20,
      HIGHSPEED_RELATIVE_SCALE = 50,
      HIGHSPEED_SCROLL_SCALE = 50,
    };
    static void fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender);

    static bool handle_button(const Params_KeyboardEventCallBack& params);
    static bool handle_move(const Params_KeyboardEventCallBack& params);
    static PointingButton getPointingButton(KeyCode keycode);

    static int dx_;
    static int dy_;
    static int scale_;
    static bool highspeed_;
    static bool scrollmode_;
    static TimerWrapper fire_timer_;
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
                            InputModeDetail inputmodedetail);
    static bool handle_RESTORE(const Params_KeyboardEventCallBack& params);

    static void firekeytoinputdetail(const Params_KeyboardEventCallBack& params,
                                     InputModeDetail inputmodedetail);

    static InputModeDetail normalize(InputModeDetail imd);

    static void fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender);
    static void fire(void);

    static InputModeDetail savedinputmodedetail_;
    static InputModeDetail currentinputmodedetail_;

    struct FireKeyInfo {
      Flags flags;
      KeyCode key;
      KeyboardType keyboardType;
      bool active;
    };
    static FireKeyInfo fireKeyInfo_;
    static TimerWrapper fire_timer_;
  };
}

#endif
