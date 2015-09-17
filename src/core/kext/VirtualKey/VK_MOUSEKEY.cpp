#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_MOUSEKEY.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_MOUSEKEY::move_down_;
bool VirtualKey::VK_MOUSEKEY::move_left_;
bool VirtualKey::VK_MOUSEKEY::move_right_;
bool VirtualKey::VK_MOUSEKEY::move_up_;
int VirtualKey::VK_MOUSEKEY::scale_;
bool VirtualKey::VK_MOUSEKEY::scrollmode_;
bool VirtualKey::VK_MOUSEKEY::highspeed_;
AutogenId VirtualKey::VK_MOUSEKEY::currentAutogenId_(0);
PhysicalEventType VirtualKey::VK_MOUSEKEY::lastPhysicalEventType_(PhysicalEventType::DOWN);
TimerWrapper VirtualKey::VK_MOUSEKEY::fire_timer_;
int VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::delta1_;
int VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::delta2_;
int VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::counter_;
AutogenId VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::autogenId_(0);
PhysicalEventType VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::physicalEventType_(PhysicalEventType::DOWN);
TimerWrapper VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::fire_timer_;

void VirtualKey::VK_MOUSEKEY::initialize(IOWorkLoop& workloop) {
  move_down_ = false;
  move_left_ = false;
  move_right_ = false;
  move_up_ = false;

  scale_ = 1;
  scrollmode_ = false;
  highspeed_ = false;
  currentAutogenId_ = AutogenId(0);
  lastPhysicalEventType_ = PhysicalEventType::DOWN;

  fire_timer_.initialize(&workloop, nullptr, VirtualKey::VK_MOUSEKEY::fire_timer_callback);

  FixedDistanceScroll::initialize(workloop);
}

void VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::initialize(IOWorkLoop& workloop) {
  delta1_ = 0;
  delta2_ = 0;
  counter_ = 0;
  autogenId_ = AutogenId(0);
  physicalEventType_ = PhysicalEventType::DOWN;

  fire_timer_.initialize(&workloop, nullptr, VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::fire_timer_callback);
}

void VirtualKey::VK_MOUSEKEY::terminate(void) {
  fire_timer_.terminate();

  FixedDistanceScroll::terminate();
}

void VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::terminate(void) {
  fire_timer_.terminate();
}

void VirtualKey::VK_MOUSEKEY::reset(void) {
  move_down_ = false;
  move_left_ = false;
  move_right_ = false;
  move_up_ = false;

  scale_ = 1;
  scrollmode_ = false;
  highspeed_ = false;
  currentAutogenId_ = AutogenId(0);
  lastPhysicalEventType_ = PhysicalEventType::DOWN;

  fire_timer_.cancelTimeout();

  FixedDistanceScroll::reset();
}

void VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::reset(void) {
  // Do nothing in order to keep scroll even if all keys are released.
}

bool VirtualKey::VK_MOUSEKEY::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  lastPhysicalEventType_ = physicalEventType;

  if (handle_button(params, autogenId, physicalEventType)) return true;
  if (handle_move(params, autogenId, physicalEventType)) return true;
  if (handle_fixeddistancemove(params, autogenId, physicalEventType)) return true;
  if (handle_fixeddistancescroll(params, autogenId, physicalEventType)) return true;
  if (handle_lock_button(params, autogenId, physicalEventType)) return true;
  return false;
}

PointingButton
VirtualKey::VK_MOUSEKEY::getPointingButton(KeyCode keycode) {
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_LEFT) {
    return PointingButton::LEFT;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_MIDDLE) {
    return PointingButton::MIDDLE;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_RIGHT) {
    return PointingButton::RIGHT;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON4) {
    return PointingButton::BUTTON4;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON5) {
    return PointingButton::BUTTON5;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON6) {
    return PointingButton::BUTTON6;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON7) {
    return PointingButton::BUTTON7;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON8) {
    return PointingButton::BUTTON8;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON9) {
    return PointingButton::BUTTON9;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON10) {
    return PointingButton::BUTTON10;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON11) {
    return PointingButton::BUTTON11;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON12) {
    return PointingButton::BUTTON12;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON13) {
    return PointingButton::BUTTON13;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON14) {
    return PointingButton::BUTTON14;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON15) {
    return PointingButton::BUTTON15;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_BUTTON_BUTTON16) {
    return PointingButton::BUTTON16;
  }
  return PointingButton::NONE;
}

PointingButton
VirtualKey::VK_MOUSEKEY::getPointingButtonFromLockKey(KeyCode keycode) {
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_LEFT) {
    return PointingButton::LEFT;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_MIDDLE) {
    return PointingButton::MIDDLE;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_RIGHT) {
    return PointingButton::RIGHT;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON4) {
    return PointingButton::BUTTON4;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON5) {
    return PointingButton::BUTTON5;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON6) {
    return PointingButton::BUTTON6;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON7) {
    return PointingButton::BUTTON7;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON8) {
    return PointingButton::BUTTON8;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON9) {
    return PointingButton::BUTTON9;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON10) {
    return PointingButton::BUTTON10;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON11) {
    return PointingButton::BUTTON11;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON12) {
    return PointingButton::BUTTON12;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON13) {
    return PointingButton::BUTTON13;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON14) {
    return PointingButton::BUTTON14;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON15) {
    return PointingButton::BUTTON15;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_BUTTON16) {
    return PointingButton::BUTTON16;
  }
  return PointingButton::NONE;
}

bool VirtualKey::VK_MOUSEKEY::isKeyLikeModifier(KeyCode keycode) {
  if (getPointingButton(keycode) != PointingButton::NONE) return true;
  if (keycode == KeyCode::VK_MOUSEKEY_UP) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_DOWN) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_LEFT) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_RIGHT) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_DIAGONAL_NE) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_DIAGONAL_NW) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_DIAGONAL_SE) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_DIAGONAL_SW) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_SCROLL_UP) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_SCROLL_DOWN) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_SCROLL_LEFT) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_SCROLL_RIGHT) {
    return true;
  }
  if (keycode == KeyCode::VK_MOUSEKEY_HIGHSPEED) {
    return true;
  }

  // VK_MOUSEKEY_FIXED_DISTANCE_* is not like modifier;
  // VK_MOUSEKEY_LOCK_BUTTON_* is not like modifier.

  return false;
}

bool VirtualKey::VK_MOUSEKEY::handle_button(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  PointingButton button = getPointingButton(params.key);
  if (button == PointingButton::NONE) return false;

  if (params.repeat) return true;

  // ----------------------------------------
  if (params.ex_iskeydown) {
    ButtonStatus::increase(button);
    EventOutputQueue::FireRelativePointer::fire(autogenId, physicalEventType, ButtonStatus::makeButtons());

  } else {
    ButtonStatus::decrease(button);
    EventOutputQueue::FireRelativePointer::fire(autogenId, physicalEventType, ButtonStatus::makeButtons());
  }

  return true;
}

bool VirtualKey::VK_MOUSEKEY::handle_move(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  /*  */ if (params.key == KeyCode::VK_MOUSEKEY_UP) {
    if (params.repeat) return true;
    move_up_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = false;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_DOWN) {
    if (params.repeat) return true;
    move_down_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = false;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_LEFT) {
    if (params.repeat) return true;
    move_left_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = false;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_RIGHT) {
    if (params.repeat) return true;
    move_right_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = false;
    }

  } else if (params.key == KeyCode::VK_MOUSEKEY_DIAGONAL_NE) {
    if (params.repeat) return true;
    move_up_ = params.ex_iskeydown;
    move_right_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = false;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_DIAGONAL_NW) {
    if (params.repeat) return true;
    move_up_ = params.ex_iskeydown;
    move_left_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = false;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_DIAGONAL_SE) {
    if (params.repeat) return true;
    move_down_ = params.ex_iskeydown;
    move_right_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = false;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_DIAGONAL_SW) {
    if (params.repeat) return true;
    move_down_ = params.ex_iskeydown;
    move_left_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = false;
    }

  } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_UP) {
    if (params.repeat) return true;
    move_up_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = true;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_DOWN) {
    if (params.repeat) return true;
    move_down_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = true;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_LEFT) {
    if (params.repeat) return true;
    move_left_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = true;
    }
  } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_RIGHT) {
    if (params.repeat) return true;
    move_right_ = params.ex_iskeydown;
    if (params.ex_iskeydown) {
      scrollmode_ = true;
    }

  } else if (params.key == KeyCode::VK_MOUSEKEY_HIGHSPEED) {
    if (params.repeat) return true;
    highspeed_ = params.ex_iskeydown;

  } else {
    return false;
  }

  currentAutogenId_ = autogenId;

  if (calculate_dx() != 0 || calculate_dy() != 0) {
    if (scrollmode_) {
      fire_timer_.setTimeoutMS(Config::get_mousekey_initial_wait_of_scroll());
    } else {
      fire_timer_.setTimeoutMS(Config::get_mousekey_initial_wait_of_pointer());
    }
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

bool VirtualKey::VK_MOUSEKEY::handle_fixeddistancemove(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
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
    EventOutputQueue::FireRelativePointer::fire(autogenId, physicalEventType,
                                                ButtonStatus::makeButtons(),
                                                fdx * Config::get_fixed_distance_magnification(),
                                                fdy * Config::get_fixed_distance_magnification());
  }

  return true;
}

bool VirtualKey::VK_MOUSEKEY::handle_fixeddistancescroll(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  int fdx = 0;
  int fdy = 0;

  /*  */ if (params.key == KeyCode::VK_MOUSEKEY_FIXED_DISTANCE_SCROLL_UP) {
    --fdy;
  } else if (params.key == KeyCode::VK_MOUSEKEY_FIXED_DISTANCE_SCROLL_DOWN) {
    ++fdy;
  } else if (params.key == KeyCode::VK_MOUSEKEY_FIXED_DISTANCE_SCROLL_LEFT) {
    --fdx;
  } else if (params.key == KeyCode::VK_MOUSEKEY_FIXED_DISTANCE_SCROLL_RIGHT) {
    ++fdx;
  } else {
    return false;
  }

  if (params.ex_iskeydown) {
    int delta1 = fdy * Config::get_fixed_distance_scroll_magnification() * EventOutputQueue::FireScrollWheel::DELTA_SCALE;
    int delta2 = fdx * Config::get_fixed_distance_scroll_magnification() * EventOutputQueue::FireScrollWheel::DELTA_SCALE;

    if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mouse_key_scroll_not_natural_direction)) {
      delta1 = -delta1;
      delta2 = -delta2;
    }

    FixedDistanceScroll::set(delta1, delta2, autogenId, physicalEventType);
  }

  return true;
}

void VirtualKey::VK_MOUSEKEY::FixedDistanceScroll::fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender) {
  if (counter_ <= 0) {
    return;
  }
  --counter_;

  EventOutputQueue::FireScrollWheel::fire(delta1_, delta2_, autogenId_, physicalEventType_);
  fire_timer_.setTimeoutMS(TIMER_INTERVAL);
}

bool VirtualKey::VK_MOUSEKEY::handle_lock_button(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  if (params.key == KeyCode::VK_MOUSEKEY_LOCK_BUTTON_ALL_FORCE_OFF) {
    ButtonStatus::lock_clear();
    EventOutputQueue::FireRelativePointer::fire(autogenId, physicalEventType, ButtonStatus::makeButtons());
    return true;
  }

  // ------------------------------------------------------------
  PointingButton button = getPointingButtonFromLockKey(params.key);
  if (button == PointingButton::NONE) return false;

  if (params.repeat) return true;

  // ----------------------------------------
  if (params.ex_iskeydown) {
    ButtonStatus::lock_toggle(button);
    EventOutputQueue::FireRelativePointer::fire(autogenId, physicalEventType, ButtonStatus::makeButtons());
  }

  return true;
}

void VirtualKey::VK_MOUSEKEY::fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender) {
  if (!scrollmode_) {
    int s = scale_;
    if (highspeed_) s = Config::get_mousekey_high_speed_of_pointer();

    EventOutputQueue::FireRelativePointer::fire(currentAutogenId_, lastPhysicalEventType_, ButtonStatus::makeButtons(), calculate_dx() * s, calculate_dy() * s);

  } else {
    int s = scale_;
    if (highspeed_) s = Config::get_mousekey_high_speed_of_scroll();

    int delta1 = calculate_dy() * s * EventOutputQueue::FireScrollWheel::DELTA_SCALE;
    int delta2 = calculate_dx() * s * EventOutputQueue::FireScrollWheel::DELTA_SCALE;

    if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mouse_key_scroll_not_natural_direction)) {
      delta1 = -delta1;
      delta2 = -delta2;
    }

    EventOutputQueue::FireScrollWheel::fire(delta1, delta2, currentAutogenId_, lastPhysicalEventType_);
  }

  int max = scrollmode_ ? Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_maximum_speed_of_scroll) : Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_maximum_speed_of_pointer);

  int acceleration = scrollmode_ ? Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_acceleration_of_scroll) : Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_acceleration_of_pointer);
  if (acceleration < 0) {
    acceleration = 0;
  }

  if (scale_ < max) {
    scale_ += acceleration;
  }

  if (scrollmode_) {
    fire_timer_.setTimeoutMS(Config::get_mousekey_repeat_wait_of_scroll());
  } else {
    fire_timer_.setTimeoutMS(Config::get_mousekey_repeat_wait_of_pointer());
  }
}
}
