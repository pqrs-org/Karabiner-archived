#include <IOKit/IOLib.h>

#include "Config.hpp"
#include "DependingPressingPeriodKeyToKey.hpp"
#include "EventWatcher.hpp"
#include "IOLogWrapper.hpp"
#include "KeyboardRepeat.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    TimerWrapper DependingPressingPeriodKeyToKey::fire_timer_;
    DependingPressingPeriodKeyToKey* DependingPressingPeriodKeyToKey::target_ = NULL;

    DependingPressingPeriodKeyToKey::PeriodMS::PeriodMS(void) : mode_(Mode::NONE)
    {
      for (size_t m = 0; m < Mode::__END__; ++m) {
        for (size_t t = 0; t < Type::__END__; ++t) {
          overwritten_value_[m][t] = -1;
        }
      }
    }

    void
    DependingPressingPeriodKeyToKey::PeriodMS::set(PeriodMS::Mode::Value newval)
    {
      mode_ = newval;
    }

    unsigned int
    DependingPressingPeriodKeyToKey::PeriodMS::get(PeriodMS::Type::Value type)
    {
      if (overwritten_value_[mode_][type] >= 0) {
        return overwritten_value_[mode_][type];
      }

      switch (mode_) {
        case Mode::HOLDING_KEY_TO_KEY:
          switch (type) {
            case Type::SHORT_PERIOD:             return Config::get_holdingkeytokey_wait();
            case Type::LONG_LONG_PERIOD:         return 0;
            case Type::PRESSING_TARGET_KEY_ONLY: return 0;
            case Type::__END__:                  return 0;
          }

        case Mode::KEY_OVERLAID_MODIFIER:
          switch (type) {
            case Type::SHORT_PERIOD:             return Config::get_keyoverlaidmodifier_initial_modifier_wait();
            case Type::LONG_LONG_PERIOD:         return 0;
            case Type::PRESSING_TARGET_KEY_ONLY: return Config::get_keyoverlaidmodifier_timeout();
            case Type::__END__:                  return 0;
          }

        case Mode::KEY_OVERLAID_MODIFIER_WITH_REPEAT:
          switch (type) {
            case Type::SHORT_PERIOD:             return Config::get_keyoverlaidmodifier_initial_modifier_wait();
            case Type::LONG_LONG_PERIOD:         return Config::get_keyoverlaidmodifier_initial_wait();
            case Type::PRESSING_TARGET_KEY_ONLY: return Config::get_keyoverlaidmodifier_timeout();
            case Type::__END__:                  return 0;
          }

        case Mode::NONE:
        case Mode::__END__:
          IOLOG_ERROR("Invalid DependingPressingPeriodKeyToKey::PeriodMS::get\n");
          return 0;
      }

      return 0;
    }

    bool
    DependingPressingPeriodKeyToKey::PeriodMS::enabled(PeriodMS::Type::Value type)
    {
      switch (mode_) {
        case Mode::HOLDING_KEY_TO_KEY:
          switch (type) {
            case Type::SHORT_PERIOD:             return true;
            case Type::LONG_LONG_PERIOD:         return false;
            case Type::PRESSING_TARGET_KEY_ONLY: return false;
            case Type::__END__:                  return false;
          }

        case Mode::KEY_OVERLAID_MODIFIER:
          switch (type) {
            case Type::SHORT_PERIOD:             return true;
            case Type::LONG_LONG_PERIOD:         return false;
            case Type::PRESSING_TARGET_KEY_ONLY: return true;
            case Type::__END__:                  return false;
          }

        case Mode::KEY_OVERLAID_MODIFIER_WITH_REPEAT:
          switch (type) {
            case Type::SHORT_PERIOD:             return true;
            case Type::LONG_LONG_PERIOD:         return true;
            case Type::PRESSING_TARGET_KEY_ONLY: return true;
            case Type::__END__:                  return false;
          }

        case Mode::NONE:
        case Mode::__END__:
          IOLOG_ERROR("Invalid DependingPressingPeriodKeyToKey::PeriodMS::enabled\n");
          return false;
      }

      return false;
    }

    // ======================================================================
    void
    DependingPressingPeriodKeyToKey::static_initialize(IOWorkLoop& workloop)
    {
      fire_timer_.initialize(&workloop, NULL, DependingPressingPeriodKeyToKey::fire_timer_callback);
    }

    void
    DependingPressingPeriodKeyToKey::static_terminate(void)
    {
      fire_timer_.terminate();
    }

    DependingPressingPeriodKeyToKey::DependingPressingPeriodKeyToKey(void) :
      active_(false),
      periodtype_(PeriodType::NONE),
      isAnyEventHappen_(false),
      keyboardRepeatID_(0),
      interruptibleByScrollWheel_(true)
    {
      for (size_t i = 0; i < KeyToKeyType::END_; ++i) {
        keytokey_[i].add(KeyCode::VK_PSEUDO_KEY);
      }
    }

    DependingPressingPeriodKeyToKey::~DependingPressingPeriodKeyToKey(void)
    {
      if (target_ == this) {
        fire_timer_.cancelTimeout();
        target_ = NULL;
      }
    }

    void
    DependingPressingPeriodKeyToKey::add(KeyToKeyType::Value type, AddDataType datatype, AddValue newval)
    {
      if (type == KeyToKeyType::END_) return;

      if (datatype == BRIDGE_DATATYPE_OPTION && Option::UNINTERRUPTIBLE_BY_SCROLL_WHEEL == Option(newval)) {
        interruptibleByScrollWheel_ = false;
      } else {
        keytokey_[type].add(datatype, newval);
      }
    }

    bool
    DependingPressingPeriodKeyToKey::remap(RemapParams& remapParams)
    {
      // Params_ScrollWheelEventCallback
      {
        Params_ScrollWheelEventCallback* params = remapParams.paramsUnion.get_Params_ScrollWheelEventCallback();
        if (params) {
          if (interruptibleByScrollWheel_) {
            dokeydown();
          }
          return false;
        }
      }

      // Params_KeyboardEventCallBack, Params_KeyboardSpecialEventCallback, Params_RelativePointerEventCallback
      {
        bool iskeydown = false;
        if (remapParams.paramsUnion.iskeydown(iskeydown)) {
          if (remapParams.isremapped ||
              ! fromEvent_.changePressingState(remapParams.paramsUnion,
                                               FlagStatus::globalFlagStatus(),
                                               fromModifierFlags_)) {
            if (iskeydown) {
              // another key is pressed.
              dokeydown();
            }
            return false;
          }

          remapParams.isremapped = true;

          if (iskeydown) {
            target_ = this;
            active_ = true;
            periodtype_ = PeriodType::NONE;

            FlagStatus::globalFlagStatus().decrease(fromEvent_.getModifierFlag());
            FlagStatus::globalFlagStatus().decrease(pureFromModifierFlags_);

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

            fire_timer_.setTimeoutMS(periodMS_.get(PeriodMS::Type::SHORT_PERIOD));

          } else {
            FlagStatus::globalFlagStatus().increase(fromEvent_.getModifierFlag());

            dokeydown();
            dokeyup();

            FlagStatus::globalFlagStatus().increase(pureFromModifierFlags_);
          }
          return true;
        }
      }

      return false;
    }

    void
    DependingPressingPeriodKeyToKey::dokeydown(void)
    {
      if (! active_) return;
      active_ = false;

      if (target_ == this) {
        fire_timer_.cancelTimeout();
      }

      switch (periodtype_) {
        case PeriodType::NONE:
        {
          periodtype_ = PeriodType::SHORT_PERIOD;

          keytokey_[KeyToKeyType::SHORT_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);

          break;
        }

        case PeriodType::SHORT_PERIOD:
        case PeriodType::LONG_PERIOD:
        case PeriodType::LONG_LONG_PERIOD:
          // do nothing
          break;
      }
    }

    void
    DependingPressingPeriodKeyToKey::dokeyup(void)
    {
      switch (periodtype_) {
        case PeriodType::SHORT_PERIOD:
        {
          periodtype_ = PeriodType::NONE;
          keytokey_[KeyToKeyType::SHORT_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          break;
        }

        case PeriodType::LONG_PERIOD:
        {
          periodtype_ = PeriodType::NONE;
          keytokey_[KeyToKeyType::LONG_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::UP);

          // ----------------------------------------
          // handle PRESSING_TARGET_KEY_ONLY
          if (periodMS_.enabled(PeriodMS::Type::PRESSING_TARGET_KEY_ONLY)) {
            if (! isAnyEventHappen_ &&
                ic_.getmillisec() < periodMS_.get(PeriodMS::Type::PRESSING_TARGET_KEY_ONLY)) {
              // Restore FlagStatus at key down.
              FlagStatus original;
              original = FlagStatus::globalFlagStatus();
              FlagStatus::globalFlagStatus() = flagStatusWhenKeyPressed_;

              keytokey_[KeyToKeyType::PRESSING_TARGET_KEY_ONLY].call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
              keytokey_[KeyToKeyType::PRESSING_TARGET_KEY_ONLY].call_remap_with_VK_PSEUDO_KEY(EventType::UP);

              // Restore current FlagStatus.
              FlagStatus::globalFlagStatus() = original;
            }
          }

          break;
        }

        case PeriodType::LONG_LONG_PERIOD:
        {
          periodtype_ = PeriodType::NONE;
          keytokey_[KeyToKeyType::LONG_LONG_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          break;
        }

        case PeriodType::NONE:
          // do nothing
          break;
      }

      EventWatcher::unset(isAnyEventHappen_);
      // FlagStatus::globalFlagStatus().increase(fromEvent_.getModifierFlag(), fromModifierFlags_);
    }

    void
    DependingPressingPeriodKeyToKey::fire_timer_callback(OSObject* owner, IOTimerEventSource* sender)
    {
      if (! target_) return;

      switch (target_->periodtype_) {
        case PeriodType::NONE:
        {
          target_->periodtype_ = PeriodType::LONG_PERIOD;

          (target_->keytokey_[KeyToKeyType::LONG_PERIOD]).call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);

          EventWatcher::set(target_->isAnyEventHappen_);
          (target_->ic_).begin();

          if (target_->periodMS_.enabled(PeriodMS::Type::LONG_LONG_PERIOD)) {
            KeyboardRepeat::cancel();
            target_->keyboardRepeatID_ = KeyboardRepeat::getID();
            fire_timer_.setTimeoutMS(target_->periodMS_.get(PeriodMS::Type::LONG_LONG_PERIOD));
          }

          break;
        }

        case PeriodType::LONG_PERIOD:
        {
          // If keyboard repeat cancellation occured while pressing LONG_PERIOD key,
          // we cancel LONG_LONG_PERIOD event.
          bool isKeyboardRepeatCanceled = (target_->keyboardRepeatID_ != KeyboardRepeat::getID());

          (target_->keytokey_[KeyToKeyType::LONG_PERIOD]).call_remap_with_VK_PSEUDO_KEY(EventType::UP);

          if (isKeyboardRepeatCanceled) {
            target_->periodtype_ = PeriodType::NONE;

          } else {
            target_->periodtype_ = PeriodType::LONG_LONG_PERIOD;

            (target_->keytokey_[KeyToKeyType::LONG_LONG_PERIOD]).call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
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
