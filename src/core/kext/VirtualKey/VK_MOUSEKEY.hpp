#ifndef VIRTUALKEY_VK_MOUSEKEY_HPP
#define VIRTUALKEY_VK_MOUSEKEY_HPP

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_MOUSEKEY final {
public:
  static void initialize(IOWorkLoop& workloop);
  static void terminate(void);
  static void reset(void);

  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  static bool isKeyLikeModifier(KeyCode keycode);

private:
  class FixedDistanceScroll {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);
    static void reset(void);

    static void set(int d1, int d2, AutogenId a, PhysicalEventType p) {
      delta1_ = d1 / MAX_COUNT;
      delta2_ = d2 / MAX_COUNT;
      counter_ = MAX_COUNT;
      autogenId_ = a;
      physicalEventType_ = p;

      fire_timer_.setTimeoutMS(TIMER_INTERVAL);
    }

  private:
    enum {
      MAX_COUNT = 100,
      TIMER_INTERVAL = 1,
    };

    static void fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender);

    static int delta1_;
    static int delta2_;
    static int counter_;
    static AutogenId autogenId_;
    static PhysicalEventType physicalEventType_;
    static TimerWrapper fire_timer_;
  };

  static void fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender);

  static bool handle_button(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  static bool handle_move(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  static bool handle_fixeddistancemove(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  static bool handle_fixeddistancescroll(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  static bool handle_lock_button(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  static PointingButton getPointingButton(KeyCode keycode);
  static PointingButton getPointingButtonFromLockKey(KeyCode keycode);

  // Manage each direction separately.
  // If we use counter like ++move_x_, --move_y_, move_x_ will not be reset accidentally even if we release all physical keys.
  // (When physical key release does not send key up event by hardware issue, the accident happen.)
  static int calculate_dx(void) { return (move_right_ ? 1 : 0) + (move_left_ ? -1 : 0); }
  static int calculate_dy(void) { return (move_down_ ? 1 : 0) + (move_up_ ? -1 : 0); }
  static bool move_down_;
  static bool move_left_;
  static bool move_right_;
  static bool move_up_;

  static int scale_;
  static bool highspeed_;
  static bool scrollmode_;
  static AutogenId currentAutogenId_;
  static PhysicalEventType lastPhysicalEventType_;
  static TimerWrapper fire_timer_;
};
}
}

#endif
