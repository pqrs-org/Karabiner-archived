#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "PressDownKeys.hpp"
#include "RemapClass.hpp"
#include "VK_IOHIDPOSTEVENT.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_Karabiner {
List EventOutputQueue::queue_;
TimerWrapper EventOutputQueue::fire_timer_;
Buttons EventOutputQueue::previousButtons_;

void EventOutputQueue::initialize(IOWorkLoop& workloop) {
  fire_timer_.initialize(&workloop, nullptr, EventOutputQueue::fire_timer_callback);
}

void EventOutputQueue::terminate(void) {
  fire_timer_.terminate();

  queue_.clear();
}

// ----------------------------------------------------------------------
#define PUSH_TO_OUTPUTQUEUE                              \
  {                                                      \
    CommonData::setcurrent_lastsentevent(p);             \
    queue_.push_back(new Item(p, AutogenId(autogenId))); \
    fire_timer_.setTimeoutMS(0, false);                  \
  }
void EventOutputQueue::push(const Params_KeyboardEventCallBack& p, AutogenId autogenId) {
  PUSH_TO_OUTPUTQUEUE;
  if (p.eventType == EventType::DOWN) {
    FlagStatus::globalFlagStatus().sticky_clear();
  }
}
void EventOutputQueue::push(const Params_KeyboardSpecialEventCallback& p, AutogenId autogenId) {
  PUSH_TO_OUTPUTQUEUE;
  if (p.ex_iskeydown) {
    FlagStatus::globalFlagStatus().sticky_clear();
  }
}
void EventOutputQueue::push(const Params_RelativePointerEventCallback& p, AutogenId autogenId) {
  PUSH_TO_OUTPUTQUEUE;
  if (p.buttons != Buttons(0)) {
    FlagStatus::globalFlagStatus().sticky_clear();
  }
}
void EventOutputQueue::push(const Params_ScrollWheelEventCallback& p, AutogenId autogenId) {
  PUSH_TO_OUTPUTQUEUE;
  FlagStatus::globalFlagStatus().sticky_clear();
}
void EventOutputQueue::push(const Params_Wait& p, AutogenId autogenId) {
  PUSH_TO_OUTPUTQUEUE;
}
#undef PUSH_TO_OUTPUTQUEUE

// ----------------------------------------------------------------------
unsigned int
EventOutputQueue::calcDelay(const Params_Base& paramsBase) {
  // ----------------------------------------
  {
    // Delay before and after modifier
    //
    // We need to wait before and after a modifier event because
    // events will be dropped by window server if
    // we send a mouse click event and a modifier event at the same time.
    //
    // For example, change middle click to command-left click by this autogen.
    // We need to put a wait on this setting in Mail.app.
    //    <autogen>
    //      __KeyToKey__
    //      PointingButton::MIDDLE, ModifierFlag::NONE,
    //      PointingButton::LEFT, ModifierFlag::COMMAND_L,
    //    </autogen>
    //
    if (paramsBase.isModifier()) {
      return Config::get_wait_before_and_after_a_modifier_key_event();
    }
  }

  // ----------------------------------------
  {
    // Delay before and after modifier
    //
    // We need to wait before and after a pointing device click event because
    // events will be dropped by processes (eg. NetBeans) if
    // we send press button event and release button event at the same time.
    //

    auto params = paramsBase.get_Params_RelativePointerEventCallback();
    if (params) {
      if (params->buttons != previousButtons_) {
        previousButtons_ = params->buttons;
        return Config::get_wait_before_and_after_a_click_event();
      }
    }
  }

  // ----------------------------------------
  return 0;
}

namespace {
unsigned int maxDelay(unsigned int v1, unsigned int v2) {
  if (v1 > v2) {
    return v1;
  } else {
    return v2;
  }
}
}

void EventOutputQueue::fire_timer_callback(OSObject* /* owner */, IOTimerEventSource* /* sender */) {
  // We need to cancelEventOutputQueueItems in fire_timer_callback
  // in order to cancel key repeat and __HoldingKeyToKey__ events.
  RemapClassManager::cancelEventOutputQueueItems();

  // Delete canceled items.
  Item* p = nullptr;

  p = static_cast<Item*>(queue_.safe_front());
  while (p) {
    if (p->isCanceled()) {
      p = static_cast<Item*>(queue_.erase_and_delete(p));
    } else {
      p = static_cast<Item*>(p->getnext());
    }
  }

  // ------------------------------------------------------------
  // Send key event.
  p = static_cast<Item*>(queue_.safe_front());
  if (!p) return;

  // Delay after modifier or click.
  unsigned int delay = calcDelay(p->getParamsBase());

  // ----------------------------------------
  {
    auto params = (p->getParamsBase()).get_Params_KeyboardEventCallBack();
    if (params) {
      bool handled = VirtualKey::handleAfterEnqueued(*params);
      if (!handled) {
        ListHookedKeyboard::instance().apply(*params);
      }

      // We need to wait at least 1ms in order to avoid changing key sequence order randomly.
      // (If VMware Fusion's driver is installed, the wrong order issue will be happen.)
      delay = maxDelay(delay, 1);
    }
  }
  {
    auto params = (p->getParamsBase()).get_Params_UpdateEventFlagsCallback();
    if (params) {
      // We need to wait at least 1ms in order to avoid changing key sequence order randomly.
      // (If VMware Fusion's driver is installed, the wrong order issue will be happen.)
      delay = maxDelay(delay, 1);
    }
  }
  {
    auto params = (p->getParamsBase()).get_Params_KeyboardSpecialEventCallback();
    if (params) {
      if (!ListHookedConsumer::instance().apply(*params)) {
        // If there is no consumer device, we send an event as a software key.
        VirtualKey::VK_IOHIDPOSTEVENT::post(*params);
      }

      // We need to wait at least 1ms in order to avoid changing key sequence order randomly.
      // (If VMware Fusion's driver is installed, the wrong order issue will be happen.)
      delay = maxDelay(delay, 1);
    }
  }
  {
    auto params = (p->getParamsBase()).get_Params_RelativePointerEventCallback();
    if (params) {
      ListHookedPointing::instance().apply(*params);
    }
  }
  {
    auto params = (p->getParamsBase()).get_Params_ScrollWheelEventCallback();
    if (params) {
      ListHookedPointing::instance().apply(*params);
    }
  }
  {
    auto params = (p->getParamsBase()).get_Params_Wait();
    if (params) {
      delay = maxDelay(delay, static_cast<unsigned int>(params->milliseconds));
    }
  }

  queue_.pop_front();

  // ----------------------------------------
  // Set timeout for next event.

  // Delay before modifier and click.
  Item* next = static_cast<Item*>(queue_.safe_front());
  if (!next) return;
  delay = maxDelay(delay, calcDelay(next->getParamsBase()));

  fire_timer_.setTimeoutMS(delay);
}

// ======================================================================
Flags EventOutputQueue::FireModifiers::lastFlags_(0);
bool EventOutputQueue::FireModifiers::isIgnorePhysicalUpEvent_(false);

void EventOutputQueue::FireModifiers::fire(AutogenId autogenId, PhysicalEventType physicalEventType, Flags toFlags, KeyboardType keyboardType) {
  if (lastFlags_ == toFlags) return;

  if (physicalEventType == PhysicalEventType::DOWN) {
    isIgnorePhysicalUpEvent_ = false;
  } else if (physicalEventType == PhysicalEventType::UP) {
    if (isIgnorePhysicalUpEvent_) {
      return;
    }
  }

  // ------------------------------------------------------------
  // At first we handle KeyUp events and handle KeyDown events next.
  // We need to end KeyDown at Command+Space to Option_L+Shift_L.
  //
  // When Option_L+Shift_L has a meaning (switch input language at Windows),
  // it does not works well when the last is KeyUp of Command.

  // ModifierFlag::NUMPAD handling.
  // (We need to remove ModifierFlag::NUMPAD at first in order to strip NUMPAD flag from normal modifier key events.)
  if (!toFlags.isOn(ModifierFlag::NUMPAD)) {
    lastFlags_.remove(ModifierFlag::NUMPAD);
  }

  // ------------------------------------------------------------
  // KeyUp
  for (size_t i = 0; i < FlagStatus::globalFlagStatus().itemSize(); ++i) {
    ModifierFlag flag = FlagStatus::globalFlagStatus().getFlag(i);

    // Skipping invalid flags
    if (flag.getKeyCode() == KeyCode::VK_NONE) continue;
    // Skipping virtual modifiers.
    if (flag.getRawBits() == 0) continue;

    // ----------------------------------------
    if (!lastFlags_.isOn(flag)) continue;
    if (toFlags.isOn(flag)) continue;

    lastFlags_.remove(flag);

    Params_KeyboardEventCallBack params(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false);
    EventOutputQueue::push(params, autogenId);
  }

  // KeyDown
  for (size_t i = 0; i < FlagStatus::globalFlagStatus().itemSize(); ++i) {
    ModifierFlag flag = FlagStatus::globalFlagStatus().getFlag(i);

    // Skipping invalid flags
    if (flag.getKeyCode() == KeyCode::VK_NONE) continue;
    // Skipping virtual modifiers.
    if (flag.getRawBits() == 0) continue;

    // ----------------------------------------
    if (!toFlags.isOn(flag)) continue;
    if (lastFlags_.isOn(flag)) continue;

    lastFlags_.add(flag);

    Params_KeyboardEventCallBack params(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false);
    EventOutputQueue::push(params, autogenId);
  }

  // ModifierFlag::NUMPAD handling.
  // (We need to add ModifierFlag::NUMPAD at last in order to strip NUMPAD flag from normal modifier key events.)
  if (toFlags.isOn(ModifierFlag::NUMPAD)) {
    lastFlags_.add(ModifierFlag::NUMPAD);
  }
}

// ======================================================================
void EventOutputQueue::FireKey::fire(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  if (VirtualKey::handle(params, autogenId, physicalEventType)) {
    CommonData::setcurrent_lastsentevent(params);
    return;
  }

  // ------------------------------------------------------------
  KeyCode newkeycode = params.key;
  Flags newflags = params.flags;

  KeyCode::reverseNormalizeKey(newkeycode, newflags, params.eventType, params.keyboardType);

  // skip no-outputable keycodes.
  // Note: check before FireModifiers to avoid meaningless modifier event.
  if (newkeycode == KeyCode::VK_NONE ||
      newkeycode == KeyCode::VK_PSEUDO_KEY) {
    // Do not call CommonData::setcurrent_lastsentevent.
    return;
  }
  if (newkeycode.isModifier() && newkeycode.getModifierFlag().getRawBits() == 0) {
    // virtual modifiers.
    CommonData::setcurrent_lastsentevent(params);
    return;
  }

  // Keep last modifier flags at EventType::UP to avoid unnecessary modifier events.
  //
  // For example, consider these rules:
  //
  //   * Change Shift-N to Control-N
  //   * Change Shift-P to Control-P
  //
  //   <autogen>__KeyToKey__ KeyCode::N, MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_SHIFT, KeyCode::N, ModifierFlag::CONTROL_L</autogen>
  //   <autogen>__KeyToKey__ KeyCode::P, MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_SHIFT, KeyCode::P, ModifierFlag::CONTROL_L</autogen>
  //
  // Case 1:
  //   Actual input:
  //     1. shift down
  //     2. p down
  //     3. p up
  //     4. n down
  //     5. n up
  //     6. shift up
  //
  //   Desirable results:
  //     1. shift down
  //     2. shift up, control down, p down
  //     3. p up
  //     4. n down
  //     5. n up
  //     6. control up
  //
  //
  // Case 2:
  //   Actual input:
  //     1. shift down
  //     2. p down
  //     3. command down
  //     4. p up
  //     5. shift up
  //     6. command up
  //
  //   Desirable results:
  //     1. shift down
  //     2. shift up, control down, p down
  //     3. control up, command down, shift down (== shift-command)
  //     4. p up
  //     5. shift up
  //     6. command up
  //
  //
  // Case 3:
  //   Actual input:
  //     1. shift down
  //     2. p down
  //     3. shift up
  //     4. p up
  //
  //   Desirable results:
  //     1. shift down
  //     2. shift up, control down, p down
  //     3. control up
  //     4. p up
  //
  //
  // Case 4:
  //   Actual input:
  //     1. shift down
  //     2. e down
  //     3. p down
  //     4. e up
  //     5. p up
  //     6. shift up
  //
  //   Desirable results:
  //     1. shift down
  //     2. e down
  //     3. shift up, control down, p down
  //     4. e up
  //     5. p up
  //     6. control up

  if (params.eventType == EventType::UP) {
    newflags = FireModifiers::getLastFlags();
  } else {
    if (params.ex_iskeydown) {
      FireModifiers::setIgnorePhysicalUpEvent(false);
    }
    FireModifiers::fire(autogenId, physicalEventType, newflags, params.keyboardType);
  }

  if (params.eventType == EventType::DOWN || params.eventType == EventType::UP) {
    Params_KeyboardEventCallBack p(params.eventType, newflags, newkeycode,
                                   params.charCode, params.charSet, params.origCharCode, params.origCharSet,
                                   params.keyboardType, params.repeat);
    EventOutputQueue::push(p, autogenId);

    if (!params.repeat) {
      if (params.eventType == EventType::DOWN) {
        PressDownKeys::add(newkeycode, params.keyboardType);
      } else {
        PressDownKeys::remove(newkeycode, params.keyboardType);
      }
    }
  }
}

// ======================================================================
void EventOutputQueue::FireConsumer::fire(const Params_KeyboardSpecialEventCallback& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  // skip no-outputable keycodes.
  // Note: check before FireModifiers to avoid meaningless modifier event.
  if (params.key == ConsumerKeyCode::VK_NONE ||
      params.key == ConsumerKeyCode::VK_PSEUDO_KEY) {
    // Do not call CommonData::setcurrent_lastsentevent.
    return;
  }

  if (params.ex_iskeydown) {
    FireModifiers::setIgnorePhysicalUpEvent(false);
  }
  FireModifiers::fire(autogenId, physicalEventType);

  EventOutputQueue::push(params, autogenId);
}

// ======================================================================
Buttons EventOutputQueue::FireRelativePointer::lastButtons_(0);

void EventOutputQueue::FireRelativePointer::fire(AutogenId autogenId, PhysicalEventType physicalEventType, Buttons toButtons, int dx, int dy) {
  // When changing space to command+left click,
  //   __KeyToKey__ KeyCode::SPACE, PointingButton::LEFT, ModifierFlag::COMMAND_L
  //
  // We need to release command key after left click was released as follows.
  //   (1) KeyDown   Command_L
  //   (2) MouseDown Left
  //   (3) MouseUp   Left
  //   (4) KeyUp     Command_L
  //
  // For mouse drag support, command modifier is increased as normal (not temporary_increase).
  // Therefore, command modifier is decreased when space key is released.
  // If we call FireModifiers::fire() at the begining of this function,
  // input events are fired as follows order.
  //   (1) KeyDown   Command_L
  //   (2) MouseDown Left
  //   (3) KeyUp     Command_L
  //   (4) MouseUp   Left
  //
  // It's not desired order.
  // So, we call FireModifiers::fire() at the end of this function when button is released.

  Buttons releasedButtons = lastButtons_;
  releasedButtons.remove(toButtons);
  bool isButtonReleased = !releasedButtons.isNONE();

  FireModifiers::setIgnorePhysicalUpEvent(false);

  if (!isButtonReleased) {
    FireModifiers::fire(autogenId, physicalEventType);
  }

  // Sending button event
  if (lastButtons_ != toButtons) {
    lastButtons_ = toButtons;

    Params_RelativePointerEventCallback params(toButtons, 0, 0, PointingButton::NONE, false);
    EventOutputQueue::push(params, autogenId);
  }

  // Sending cursor
  if (dx != 0 || dy != 0) {
    Params_RelativePointerEventCallback params(toButtons, dx, dy, PointingButton::NONE, false);
    EventOutputQueue::push(params, autogenId);
  }

  if (isButtonReleased) {
    FireModifiers::fire(autogenId, physicalEventType);
  }
}

// ======================================================================
void EventOutputQueue::FireScrollWheel::fire(const Params_ScrollWheelEventCallback& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  FireModifiers::setIgnorePhysicalUpEvent(false);
  FireModifiers::fire(autogenId, physicalEventType);
  EventOutputQueue::push(params, autogenId);
}

void EventOutputQueue::FireScrollWheel::fire(int delta1, int delta2, AutogenId autogenId, PhysicalEventType physicalEventType) {
  short deltaAxis1;
  short deltaAxis2;
  IOFixed fixedDelta1;
  IOFixed fixedDelta2;
  SInt32 pointDelta1;
  SInt32 pointDelta2;

  int relative2scroll_rate = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_pointing_relative2scroll_rate);

  deltaAxis1 = (delta1 * relative2scroll_rate) / 1024 / DELTA_SCALE;
  deltaAxis2 = (delta2 * relative2scroll_rate) / 1024 / DELTA_SCALE;

  fixedDelta1 = (delta1 * relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024) / DELTA_SCALE;
  fixedDelta2 = (delta2 * relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024) / DELTA_SCALE;

  pointDelta1 = (delta1 * POINTING_POINT_SCALE * relative2scroll_rate) / 1024 / DELTA_SCALE;
  pointDelta2 = (delta2 * POINTING_POINT_SCALE * relative2scroll_rate) / 1024 / DELTA_SCALE;

  // see IOHIDSystem/IOHIDevicePrivateKeys.h about options.
  const int kScrollTypeContinuous_ = 0x0001;
  int options = kScrollTypeContinuous_;

  Params_ScrollWheelEventCallback params(deltaAxis1, deltaAxis2, 0,
                                         fixedDelta1, fixedDelta2, 0,
                                         pointDelta1, pointDelta2, 0,
                                         options);
  EventOutputQueue::FireScrollWheel::fire(params, autogenId, physicalEventType);
}

// ======================================================================
void EventOutputQueue::FireWait::fire(const Params_Wait& params, AutogenId autogenId) {
  EventOutputQueue::push(params, autogenId);
}
}
