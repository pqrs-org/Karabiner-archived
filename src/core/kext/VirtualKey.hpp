#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"
#include "RemapClass.hpp"
#include "TimerWrapper.hpp"
#include "Vector.hpp"
#include "VirtualKey/VK_CHANGE_INPUTMODE.hpp"
#include "VirtualKey/VK_CONFIG.hpp"
#include "VirtualKey/VK_LAZY.hpp"
#include "VirtualKey/VK_LOCK.hpp"
#include "VirtualKey/VK_MOUSEKEY.hpp"
#include "VirtualKey/VK_STICKY.hpp"
#include "VirtualKey/VK_JIS_TOGGLE_EISUU_KANA.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    void initialize(IOWorkLoop& workloop);
    void terminate(void);
    void reset(void);

    bool isKeyLikeModifier(KeyCode keycode);
  };

  // ----------------------------------------------------------------------
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
