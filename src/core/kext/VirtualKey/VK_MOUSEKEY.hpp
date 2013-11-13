#ifndef VIRTUALKEY_VK_MOUSEKEY_HPP
#define VIRTUALKEY_VK_MOUSEKEY_HPP

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_MOUSEKEY {
    public:
      static void initialize(IOWorkLoop& workloop);
      static void terminate(void);
      static void reset(void);

      static bool handle(const Params_KeyboardEventCallBack& params);
      static bool isKeyLikeModifier(KeyCode keycode);

    private:
      enum {
        TIMER_INTERVAL = 20,
      };
      static void fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender);

      static bool handle_button(const Params_KeyboardEventCallBack& params);
      static bool handle_move(const Params_KeyboardEventCallBack& params);
      static bool handle_fixeddistancemove(const Params_KeyboardEventCallBack& params);
      static bool handle_lock_button(const Params_KeyboardEventCallBack& params);
      static PointingButton getPointingButton(KeyCode keycode);
      static PointingButton getPointingButtonFromLockKey(KeyCode keycode);

      static int dx_;
      static int dy_;
      static int scale_;
      static bool highspeed_;
      static bool scrollmode_;
      static TimerWrapper fire_timer_;
    };
  }
}

#endif
