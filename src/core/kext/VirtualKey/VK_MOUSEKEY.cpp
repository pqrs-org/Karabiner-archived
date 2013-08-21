#include "base.hpp"
#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_MOUSEKEY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  int VirtualKey::VK_MOUSEKEY::dx_;
  int VirtualKey::VK_MOUSEKEY::dy_;
  int VirtualKey::VK_MOUSEKEY::scale_;
  bool VirtualKey::VK_MOUSEKEY::scrollmode_;
  bool VirtualKey::VK_MOUSEKEY::highspeed_;
  TimerWrapper VirtualKey::VK_MOUSEKEY::fire_timer_;

  void
  VirtualKey::VK_MOUSEKEY::initialize(IOWorkLoop& workloop)
  {
    dx_ = 0;
    dy_ = 0;
    scale_ = 1;
    scrollmode_ = false;
    highspeed_ = false;

    fire_timer_.initialize(&workloop, NULL, VirtualKey::VK_MOUSEKEY::fire_timer_callback);
  }

  void
  VirtualKey::VK_MOUSEKEY::terminate(void)
  {
    fire_timer_.terminate();
  }

  void
  VirtualKey::VK_MOUSEKEY::reset(void)
  {
    dx_ = 0;
    dy_ = 0;
    scale_ = 1;
    scrollmode_ = false;
    highspeed_ = false;

    fire_timer_.cancelTimeout();
  }

  bool
  VirtualKey::VK_MOUSEKEY::handle(const Params_KeyboardEventCallBack& params)
  {
    if (handle_button(params)) return true;
    if (handle_move(params)) return true;
    if (handle_fixeddistancemove(params)) return true;
    if (handle_lock_button(params)) return true;
    return false;
  }

  PointingButton
  VirtualKey::VK_MOUSEKEY::getPointingButton(KeyCode keycode)
  {
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_LEFT)     { return PointingButton::LEFT;     }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_MIDDLE)   { return PointingButton::MIDDLE;   }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_RIGHT)    { return PointingButton::RIGHT;    }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON4)  { return PointingButton::BUTTON4;  }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON5)  { return PointingButton::BUTTON5;  }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON6)  { return PointingButton::BUTTON6;  }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON7)  { return PointingButton::BUTTON7;  }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON8)  { return PointingButton::BUTTON8;  }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON9)  { return PointingButton::BUTTON9;  }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON10) { return PointingButton::BUTTON10; }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON11) { return PointingButton::BUTTON11; }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON12) { return PointingButton::BUTTON12; }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON13) { return PointingButton::BUTTON13; }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON14) { return PointingButton::BUTTON14; }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON15) { return PointingButton::BUTTON15; }
    if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON16) { return PointingButton::BUTTON16; }
    return PointingButton::NONE;
  }

  PointingButton
  VirtualKey::VK_MOUSEKEY::getPointingButtonFromLockKey(KeyCode keycode)
  {
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_LEFT)     { return PointingButton::LEFT;     }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_MIDDLE)   { return PointingButton::MIDDLE;   }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_RIGHT)    { return PointingButton::RIGHT;    }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON4)  { return PointingButton::BUTTON4;  }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON5)  { return PointingButton::BUTTON5;  }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON6)  { return PointingButton::BUTTON6;  }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON7)  { return PointingButton::BUTTON7;  }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON8)  { return PointingButton::BUTTON8;  }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON9)  { return PointingButton::BUTTON9;  }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON10) { return PointingButton::BUTTON10; }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON11) { return PointingButton::BUTTON11; }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON12) { return PointingButton::BUTTON12; }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON13) { return PointingButton::BUTTON13; }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON14) { return PointingButton::BUTTON14; }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON15) { return PointingButton::BUTTON15; }
    if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON16) { return PointingButton::BUTTON16; }
    return PointingButton::NONE;
  }

  bool
  VirtualKey::VK_MOUSEKEY::isKeyLikeModifier(KeyCode keycode)
  {
    if (getPointingButton(keycode) != PointingButton::NONE) return true;
    if (keycode == KeyCode::VK_MOUSEKEY_UP)           { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_DOWN)         { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_LEFT)         { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_RIGHT)        { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_DIAGONAL_NE)  { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_DIAGONAL_NW)  { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_DIAGONAL_SE)  { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_DIAGONAL_SW)  { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_SCROLL_UP)    { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_SCROLL_DOWN)  { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_SCROLL_LEFT)  { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_SCROLL_RIGHT) { return true; }
    if (keycode == KeyCode::VK_MOUSEKEY_HIGHSPEED)    { return true; }

    // VK_MOUSEKEY_FIXED_DISTANCE_* is not like modifier;
    // VK_MOUSEKEY_LOCK_BUTTON_* is not like modifier.

    return false;
  }

  bool
  VirtualKey::VK_MOUSEKEY::handle_button(const Params_KeyboardEventCallBack& params)
  {
    PointingButton button = getPointingButton(params.key);
    if (button == PointingButton::NONE) return false;

    if (params.repeat) return true;

    // ----------------------------------------
    if (params.ex_iskeydown) {
      ButtonStatus::increase(button);
      EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());

    } else {
      ButtonStatus::decrease(button);
      EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());
    }

    return true;
  }

  bool
  VirtualKey::VK_MOUSEKEY::handle_move(const Params_KeyboardEventCallBack& params)
  {
    /*  */ if (params.key == KeyCode::VK_MOUSEKEY_UP) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dy_; scrollmode_ = false; } else { ++dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_DOWN) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dy_; scrollmode_ = false; } else { --dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_LEFT) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dx_; scrollmode_ = false; } else { ++dx_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_RIGHT) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dx_; scrollmode_ = false; } else { --dx_; }
      }

    } else if (params.key == KeyCode::VK_MOUSEKEY_DIAGONAL_NE) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dx_; --dy_; scrollmode_ = false; } else { --dx_; ++dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_DIAGONAL_NW) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dx_; --dy_; scrollmode_ = false; } else { ++dx_; ++dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_DIAGONAL_SE) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dx_; ++dy_; scrollmode_ = false; } else { --dx_; --dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_DIAGONAL_SW) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dx_; ++dy_; scrollmode_ = false; } else { ++dx_; --dy_; }
      }

    } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_UP) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dy_; scrollmode_ = true; } else { ++dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_DOWN) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dy_; scrollmode_ = true; } else { --dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_LEFT) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dx_; scrollmode_ = true; } else { ++dx_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_RIGHT) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dx_; scrollmode_ = true; } else { --dx_; }
      }

    } else if (params.key == KeyCode::VK_MOUSEKEY_HIGHSPEED) {
      if (params.repeat == false) {
        highspeed_ = params.ex_iskeydown;
      }

    } else {
      return false;
    }

    if (dx_ != 0 || dy_ != 0) {
      fire_timer_.setTimeoutMS(TIMER_INTERVAL, false);
    } else {
      scale_ = 1;

      // keep scrollmode_ & highspeed_.
      //
      // When VK_MOUSEKEY_SCROLL_UP and VK_MOUSEKEY_SCROLL_DOWN are pressed at the same time,
      // this code will be executed.
      //
      // In the above case, we need to keep scrollmode_, highspeed_ value.

      fire_timer_.cancelTimeout();
    }

    return true;
  }

  bool
  VirtualKey::VK_MOUSEKEY::handle_fixeddistancemove(const Params_KeyboardEventCallBack& params)
  {
    int fdx = 0;
    int fdy = 0;

    /*  */ if (params.key == KeyCode::VK_MOUSEKEY_FIXED_DISTANCE_UP) {
      --fdy;
    } else if (params.key == KeyCode::VK_MOUSEKEY_FIXED_DISTANCE_DOWN) {
      ++fdy;
    } else if (params.key == KeyCode::VK_MOUSEKEY_FIXED_DISTANCE_LEFT) {
      --fdx;
    } else if (params.key == KeyCode::VK_MOUSEKEY_FIXED_DISTANCE_RIGHT) {
      ++fdx;
    } else {
      return false;
    }

    if (params.ex_iskeydown) {
      EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons(),
                                                  fdx * Config::get_fixed_distance_magnification(),
                                                  fdy * Config::get_fixed_distance_magnification());
    }

    return true;
  }

  bool
  VirtualKey::VK_MOUSEKEY::handle_lock_button(const Params_KeyboardEventCallBack& params)
  {
    if (params.key == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_ALL_FORCE_OFF) {
      ButtonStatus::lock_clear();
      EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());
      return true;
    }

    // ------------------------------------------------------------
    PointingButton button = getPointingButtonFromLockKey(params.key);
    if (button == PointingButton::NONE) return false;

    if (params.repeat) return true;

    // ----------------------------------------
    if (params.ex_iskeydown) {
      ButtonStatus::lock_toggle(button);
      EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());
    }

    return true;
  }

  void
  VirtualKey::VK_MOUSEKEY::fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender)
  {
    if (! scrollmode_) {
      int s = scale_;
      if (highspeed_) s = HIGHSPEED_RELATIVE_SCALE;

      EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons(), dx_ * s, dy_ * s);

    } else {
      int s = scale_;
      if (highspeed_) s = HIGHSPEED_SCROLL_SCALE;

      int delta1 = -dy_ * s * EventOutputQueue::FireScrollWheel::DELTA_SCALE;
      int delta2 = -dx_ * s * EventOutputQueue::FireScrollWheel::DELTA_SCALE;

      if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mouse_key_scroll_natural_direction)) {
        delta1 = - delta1;
      }

      EventOutputQueue::FireScrollWheel::fire(delta1, delta2);
    }

    int max = scrollmode_ ?
              Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_maximum_speed_of_scroll) :
              Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_maximum_speed_of_pointer);

    int acceleration = scrollmode_ ?
                       Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_acceleration_of_scroll) :
                       Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_acceleration_of_pointer);
    if (acceleration < 0) {
      acceleration = 0;
    }

    if (scale_ < max) {
      scale_ += acceleration;
    }

    fire_timer_.setTimeoutMS(TIMER_INTERVAL);
  }
}
