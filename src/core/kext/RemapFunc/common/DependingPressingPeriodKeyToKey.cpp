#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "Config.hpp"
#include "DependingPressingPeriodKeyToKey.hpp"
#include "IOLogWrapper.hpp"
#include "KeyboardRepeat.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
TimerWrapper DependingPressingPeriodKeyToKey::fire_timer_;
DependingPressingPeriodKeyToKey* DependingPressingPeriodKeyToKey::target_ = nullptr;

DependingPressingPeriodKeyToKey::PeriodMS::PeriodMS(void) : mode_(Mode::NONE) {
  for (size_t m = 0; m < Mode::__END__; ++m) {
    for (size_t t = 0; t < Type::__END__; ++t) {
      overwritten_value_[m][t] = -1;
    }
  }
}

void DependingPressingPeriodKeyToKey::PeriodMS::set(PeriodMS::Mode::Value newval) {
  mode_ = newval;
}

unsigned int
DependingPressingPeriodKeyToKey::PeriodMS::get(PeriodMS::Type::Value type) {
  if (overwritten_value_[mode_][type] >= 0) {
    return overwritten_value_[mode_][type];
  }

  switch (mode_) {
  case Mode::HOLDING_KEY_TO_KEY:
    switch (type) {
    case Type::SHORT_PERIOD:
      return Config::get_holdingkeytokey_wait();
    case Type::LONG_LONG_PERIOD:
      return 0;
    case Type::PRESSING_TARGET_KEY_ONLY:
      return 0;
    case Type::__END__:
      return 0;
    }

  case Mode::KEY_OVERLAID_MODIFIER:
    switch (type) {
    case Type::SHORT_PERIOD:
      return Config::get_keyoverlaidmodifier_initial_modifier_wait();
    case Type::LONG_LONG_PERIOD:
      return 0;
    case Type::PRESSING_TARGET_KEY_ONLY:
      return Config::get_keyoverlaidmodifier_timeout();
    case Type::__END__:
      return 0;
    }

  case Mode::KEY_OVERLAID_MODIFIER_WITH_REPEAT:
    switch (type) {
    case Type::SHORT_PERIOD:
      return Config::get_keyoverlaidmodifier_initial_modifier_wait();
    case Type::LONG_LONG_PERIOD:
      return Config::get_keyoverlaidmodifier_initial_wait();
    case Type::PRESSING_TARGET_KEY_ONLY:
      return Config::get_keyoverlaidmodifier_timeout();
    case Type::__END__:
      return 0;
    }

  case Mode::NONE:
  case Mode::__END__:
    IOLOG_ERROR("Invalid DependingPressingPeriodKeyToKey::PeriodMS::get\n");
    return 0;
  }

  return 0;
}

bool DependingPressingPeriodKeyToKey::PeriodMS::enabled(PeriodMS::Type::Value type) {
  switch (mode_) {
  case Mode::HOLDING_KEY_TO_KEY:
    switch (type) {
    case Type::SHORT_PERIOD:
      return true;
    case Type::LONG_LONG_PERIOD:
      return false;
    case Type::PRESSING_TARGET_KEY_ONLY:
      return false;
    case Type::__END__:
      return false;
    }

  case Mode::KEY_OVERLAID_MODIFIER:
    switch (type) {
    case Type::SHORT_PERIOD:
      return true;
    case Type::LONG_LONG_PERIOD:
      return false;
    case Type::PRESSING_TARGET_KEY_ONLY:
      return true;
    case Type::__END__:
      return false;
    }

  case Mode::KEY_OVERLAID_MODIFIER_WITH_REPEAT:
    switch (type) {
    case Type::SHORT_PERIOD:
      return true;
    case Type::LONG_LONG_PERIOD:
      return true;
    case Type::PRESSING_TARGET_KEY_ONLY:
      return true;
    case Type::__END__:
      return false;
    }

  case Mode::NONE:
  case Mode::__END__:
    IOLOG_ERROR("Invalid DependingPressingPeriodKeyToKey::PeriodMS::enabled\n");
    return false;
  }

  return false;
}

// ======================================================================
void DependingPressingPeriodKeyToKey::static_initialize(IOWorkLoop& workloop) {
  fire_timer_.initialize(&workloop, nullptr, DependingPressingPeriodKeyToKey::fire_timer_callback);
}

void DependingPressingPeriodKeyToKey::static_terminate(void) {
  fire_timer_.terminate();
}

DependingPressingPeriodKeyToKey::DependingPressingPeriodKeyToKey(RemapFunc::RemapFuncBase* owner, AutogenId autogenId) : owner_(owner),
                                                                                                                         active_(false),
                                                                                                                         periodtype_(PeriodType::NONE),
                                                                                                                         isFromModifierDecreased_(false),
                                                                                                                         keytokey_{{autogenId},
                                                                                                                                   {autogenId},
                                                                                                                                   {autogenId},
                                                                                                                                   {autogenId}},
                                                                                                                         beforeAfterKeys_(autogenId),
                                                                                                                         keyboardRepeatID_(0),
                                                                                                                         lastPhysicalEventType_(PhysicalEventType::DOWN),
                                                                                                                         interruptibleByScrollWheel_(true) {
  for (size_t i = 0; i < KeyToKeyType::END_; ++i) {
    keytokey_[i].add(KeyCode::VK_PSEUDO_KEY);
  }
  beforeAfterKeys_.add(KeyCode::VK_PSEUDO_KEY);
}

DependingPressingPeriodKeyToKey::~DependingPressingPeriodKeyToKey(void) {
  if (target_ == this) {
    fire_timer_.cancelTimeout();
    target_ = nullptr;
  }
}

void DependingPressingPeriodKeyToKey::add(KeyToKeyType::Value type, AddDataType datatype, AddValue newval) {
  if (type == KeyToKeyType::END_) return;

  if (datatype == BRIDGE_DATATYPE_OPTION && Option::UNINTERRUPTIBLE_BY_SCROLL_WHEEL == Option(newval)) {
    interruptibleByScrollWheel_ = false;
  } else {
    keytokey_[type].add(datatype, newval);
  }
}

void DependingPressingPeriodKeyToKey::prepare(RemapParams& remapParams) {
  // Params_ScrollWheelEventCallback
  {
    auto params = remapParams.paramsBase.get_Params_ScrollWheelEventCallback();
    if (params) {
      if (interruptibleByScrollWheel_) {
        dokeydown(remapParams);
      }
    }
  }

  // Params_KeyboardEventCallBack, Params_KeyboardSpecialEventCallback, Params_RelativePointerEventCallback
  {
    bool iskeydown = false;
    if (remapParams.paramsBase.iskeydown(iskeydown)) {
      if (iskeydown) {
        // another key is pressed.
        dokeydown(remapParams);
      }
    }
  }
}

bool DependingPressingPeriodKeyToKey::remap(RemapParams& remapParams) {
  // Params_KeyboardEventCallBack, Params_KeyboardSpecialEventCallback, Params_RelativePointerEventCallback
  {
    bool iskeydown = false;
    if (remapParams.paramsBase.iskeydown(iskeydown)) {
      if (remapParams.isremapped ||
          !fromEvent_.changePressingState(remapParams.paramsBase,
                                          FlagStatus::globalFlagStatus(),
                                          fromModifierFlags_)) {
        return false;
      }

      remapParams.isremapped = true;
      lastPhysicalEventType_ = remapParams.physicalEventType;

      if (iskeydown) {
        target_ = this;
        active_ = true;
        periodtype_ = PeriodType::NONE;

        ButtonStatus::decrease(fromEvent_.getPointingButton());
        FlagStatus::globalFlagStatus().decrease(fromEvent_.getModifierFlag());

        manipulatePureFromModifierFlags(beforeAfterKeys_, Manipulation::DECREASE, true);
        {
          beforeAfterKeys_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, remapParams.physicalEventType);

          // We need save FlagStatus at keydown.
          // For example, "Change Space to Shift_L" is enabled,
          //
          // (1) Control_L down
          // (2) Space down
          // (3) Control_L up
          // (4) Space up        -> We should send Control_L+Space here.
          //
          // At (4), Control_L is not presssed.
          // We should send Control_L+Space at (4) because user pressed Space with Control_L at (2).
          //
          // Therefore, we need to save FlagStatus at (2).
          // And restore it at (4).
          //
          flagStatusWhenKeyPressed_ = FlagStatus::globalFlagStatus();

          // Stop modifier flag events until normal key event is happen or physical key down event is happen
          // in order to reduce unnecessary modifier events.
          //
          // For example, we consider the following autogens:
          //
          //    <autogen>
          //      __KeyOverlaidModifier__
          //      KeyCode::COMMAND_L,
          //      KeyCode::CONTROL_L,
          //      KeyCode::ESCAPE,
          //    </autogen>
          //    <autogen>
          //      __KeyOverlaidModifier__
          //      KeyCode::SPACE,
          //      KeyCode::SHIFT_L,
          //      KeyCode::SPACE,
          //    </autogen>
          //
          // * Change the left command key to control key.
          // * Change the space key to shift key.
          // * Send an escape key event when the left command key is pressed alone,
          // * Send a space key event when the space key is pressed alone.
          //
          // When we press keys by this order.
          // #1 Press COMMAND_L.
          // #2 Press SPACE.
          // #3 Release COMMAND_L.
          // #4 Release SPACE.
          //
          // The result is control-space at #4.
          //
          // If we does not stop modifier events, CONTROL_L events are sent at #3 and #4.
          //
          // #1 -> CONTROL_L down.
          // #2 -> SHIFT_L down.
          // #3 -> CONTROL_L up.
          // #4 -> CONTROL_L down, SHIFT_L up, SPACE down, SPACE up, CONTROL_L up.
          //
          // To reduce these events, we ignore modifier events at physical key up.
          //
          // #1 -> CONTROL_L down
          // #2 -> SHIFT_L down.
          // #3 -> Do nothing.
          // #4 -> SHIFT_L up, SPACE down, SPACE up, CONTROL_L up.
          //

          EventOutputQueue::FireModifiers::setIgnorePhysicalUpEvent(true);
        }
        manipulatePureFromModifierFlags(beforeAfterKeys_, Manipulation::INCREASE, true);

        unsigned int ms = periodMS_.get(PeriodMS::Type::SHORT_PERIOD);
        if (ms == 0) {
          fire_timer_callback(nullptr, nullptr);
        } else {
          fire_timer_.setTimeoutMS(ms);
        }

        RemapClassManager::registerPrepareTargetItem(owner_);

      } else {
        // Restore fromEvent_ state to event firing in `dokeyup` for __KeyOverlaidModifier__.
        //
        // For example, we have to restore PointingButton::RIGHT before `dokeyup` for this autogen:
        //
        // <autogen>
        //   __KeyOverlaidModifier__
        //   PointingButton::RIGHT,
        //   KeyCode::VK_MODIFIER_BUTTON2,
        //   PointingButton::RIGHT,
        // </autogen>

        ButtonStatus::increase(fromEvent_.getPointingButton());
        FlagStatus::globalFlagStatus().increase(fromEvent_.getModifierFlag());

        dokeydown(remapParams);
        dokeyup();

        manipulatePureFromModifierFlags(beforeAfterKeys_, Manipulation::DECREASE, true);
        beforeAfterKeys_.call_remap_with_VK_PSEUDO_KEY(EventType::UP, remapParams.physicalEventType);
        manipulatePureFromModifierFlags(beforeAfterKeys_, Manipulation::INCREASE, true);

        RemapClassManager::unregisterPrepareTargetItem(owner_);
      }
      return true;
    }
  }

  return false;
}

void DependingPressingPeriodKeyToKey::manipulatePureFromModifierFlags(const KeyToKey& keytokey, Manipulation manipulation, bool force) {
  // already decreased.
  if (manipulation == Manipulation::DECREASE && isFromModifierDecreased_) {
    return;
  }
  // not decreased.
  if (manipulation == Manipulation::INCREASE && !isFromModifierDecreased_) {
    return;
  }

  bool needToManipulate = false;

  if (force) {
    needToManipulate = true;
  } else {
    if (keytokey.isLastToEventModifierKeyOrLikeModifier()) {
      // to modifier
      if (manipulation == Manipulation::DECREASE) {
        needToManipulate = true;
      } else {
        needToManipulate = false;
      }
    } else {
      // to key
      needToManipulate = true;
    }
  }

  if (needToManipulate) {
    switch (manipulation) {
    case Manipulation::DECREASE:
      isFromModifierDecreased_ = true;
      FlagStatus::globalFlagStatus().decrease(pureFromModifierFlags_);
      break;

    case Manipulation::INCREASE:
      isFromModifierDecreased_ = false;
      FlagStatus::globalFlagStatus().increase(pureFromModifierFlags_);
      break;
    }
  }
}

void DependingPressingPeriodKeyToKey::dokeydown(RemapParams& remapParams) {
  if (!active_) return;
  active_ = false;

  if (target_ == this) {
    fire_timer_.cancelTimeout();
  }

  switch (periodtype_) {
  case PeriodType::NONE: {
    periodtype_ = PeriodType::SHORT_PERIOD;

    {
      auto& ktk = keytokey_[KeyToKeyType::SHORT_PERIOD];
      manipulatePureFromModifierFlags(ktk, Manipulation::DECREASE);
      {
        FlagStatus::ScopedSetter scopedSetter(FlagStatus::globalFlagStatus(), flagStatusWhenKeyPressed_);

        ktk.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, remapParams.physicalEventType);

        // Call prepare in order to cancel delayed action.
        //
        // For example:
        //   1. Enable remap.samples_keytokey_delayed_action_3 in samples.xml.
        //   2. Type return.
        //   3. Type space.
        //   4. It should be changed to 1,space.
        //
        // If we don't call prepare, the delayed action will be registered when we type the space key
        // and it will not be canceled.
        // The result becomes `space,1`. (`1` is entered by delayed action after space key.)
        //
        // Therefore we need to call prepare to preserve events order.

        ktk.prepare(remapParams);
      }
      manipulatePureFromModifierFlags(ktk, Manipulation::INCREASE);
    }

    break;
  }

  case PeriodType::SHORT_PERIOD:
  case PeriodType::LONG_PERIOD:
  case PeriodType::LONG_LONG_PERIOD:
    // do nothing
    break;
  }
}

void DependingPressingPeriodKeyToKey::dokeyup(void) {
  switch (periodtype_) {
  case PeriodType::SHORT_PERIOD: {
    periodtype_ = PeriodType::NONE;
    {
      auto& ktk = keytokey_[KeyToKeyType::SHORT_PERIOD];
      ktk.call_remap_with_VK_PSEUDO_KEY(EventType::UP, lastPhysicalEventType_);
      manipulatePureFromModifierFlags(ktk, Manipulation::INCREASE);
    }
    break;
  }

  case PeriodType::LONG_PERIOD: {
    periodtype_ = PeriodType::NONE;
    {
      auto& ktk = keytokey_[KeyToKeyType::LONG_PERIOD];
      ktk.call_remap_with_VK_PSEUDO_KEY(EventType::UP, lastPhysicalEventType_);
      manipulatePureFromModifierFlags(ktk, Manipulation::INCREASE);
    }

    // ----------------------------------------
    // handle PRESSING_TARGET_KEY_ONLY
    if (periodMS_.enabled(PeriodMS::Type::PRESSING_TARGET_KEY_ONLY)) {
      if (!eventWatcherTarget_.isAnyEventHappen() &&
          ic_.getmillisec() < periodMS_.get(PeriodMS::Type::PRESSING_TARGET_KEY_ONLY)) {

        auto& ktk = keytokey_[KeyToKeyType::PRESSING_TARGET_KEY_ONLY];
        manipulatePureFromModifierFlags(ktk, Manipulation::DECREASE);
        {
          FlagStatus::ScopedSetter scopedSetter(FlagStatus::globalFlagStatus(), flagStatusWhenKeyPressed_);
          ktk.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, lastPhysicalEventType_);
          ktk.call_remap_with_VK_PSEUDO_KEY(EventType::UP, lastPhysicalEventType_);
        }
        manipulatePureFromModifierFlags(ktk, Manipulation::INCREASE);
      }
    }

    break;
  }

  case PeriodType::LONG_LONG_PERIOD: {
    periodtype_ = PeriodType::NONE;
    {
      auto& ktk = keytokey_[KeyToKeyType::LONG_LONG_PERIOD];
      ktk.call_remap_with_VK_PSEUDO_KEY(EventType::UP, lastPhysicalEventType_);
      manipulatePureFromModifierFlags(ktk, Manipulation::INCREASE);
    }
    break;
  }

  case PeriodType::NONE:
    // do nothing
    break;
  }

  eventWatcherTarget_.unobserve();
}

void DependingPressingPeriodKeyToKey::fire_timer_callback(OSObject* /* owner */, IOTimerEventSource* /* sender */) {
  if (!target_) return;

  switch (target_->periodtype_) {
  case PeriodType::NONE: {
    target_->periodtype_ = PeriodType::LONG_PERIOD;

    {
      auto& ktk = target_->keytokey_[KeyToKeyType::LONG_PERIOD];
      target_->manipulatePureFromModifierFlags(ktk, Manipulation::DECREASE);
      {
        FlagStatus::ScopedSetter scopedSetter(FlagStatus::globalFlagStatus(), target_->flagStatusWhenKeyPressed_);
        ktk.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, target_->lastPhysicalEventType_);
      }
      target_->manipulatePureFromModifierFlags(ktk, Manipulation::INCREASE);
    }

    (target_->eventWatcherTarget_).observe();
    (target_->ic_).begin();

    if (target_->periodMS_.enabled(PeriodMS::Type::LONG_LONG_PERIOD)) {
      KeyboardRepeat::cancel();
      target_->keyboardRepeatID_ = KeyboardRepeat::getID();
      fire_timer_.setTimeoutMS(target_->periodMS_.get(PeriodMS::Type::LONG_LONG_PERIOD));
    }

    break;
  }

  case PeriodType::LONG_PERIOD: {
    // If keyboard repeat cancellation occured while pressing LONG_PERIOD key,
    // we cancel LONG_LONG_PERIOD event.
    bool isKeyboardRepeatCanceled = (target_->keyboardRepeatID_ != KeyboardRepeat::getID());

    {
      auto& ktk = (target_->keytokey_[KeyToKeyType::LONG_PERIOD]);
      ktk.call_remap_with_VK_PSEUDO_KEY(EventType::UP, target_->lastPhysicalEventType_);
      target_->manipulatePureFromModifierFlags(ktk, Manipulation::INCREASE);
    }

    if (isKeyboardRepeatCanceled) {
      target_->periodtype_ = PeriodType::NONE;

    } else {
      target_->periodtype_ = PeriodType::LONG_LONG_PERIOD;

      {
        auto& ktk = (target_->keytokey_[KeyToKeyType::LONG_LONG_PERIOD]);
        target_->manipulatePureFromModifierFlags(ktk, Manipulation::DECREASE);
        {
          FlagStatus::ScopedSetter scopedSetter(FlagStatus::globalFlagStatus(), target_->flagStatusWhenKeyPressed_);
          (target_->keytokey_[KeyToKeyType::LONG_LONG_PERIOD]).call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, target_->lastPhysicalEventType_);
        }
        target_->manipulatePureFromModifierFlags(ktk, Manipulation::INCREASE);
      }
    }

    break;
  }

  case PeriodType::SHORT_PERIOD:
  case PeriodType::LONG_LONG_PERIOD:
    // do nothing
    break;
  }
}
}
}
