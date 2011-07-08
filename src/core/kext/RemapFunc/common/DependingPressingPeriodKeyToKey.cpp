#include "Config.hpp"
#include "DependingPressingPeriodKeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    TimerWrapper DependingPressingPeriodKeyToKey::fire_timer_;
    DependingPressingPeriodKeyToKey* DependingPressingPeriodKeyToKey::target_ = NULL;

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
      active_(false), periodtype_(PeriodType::NONE)
    {
      for (int i = 0; i < PeriodType::END_; ++i) {
        periodMS_[i] = 0;
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
    DependingPressingPeriodKeyToKey::add(KeyToKeyType::Value type, unsigned int datatype, unsigned int newval)
    {
      if (type == KeyToKeyType::END_) return;
      keytokey_[type].add(datatype, newval);
    }

    void
    DependingPressingPeriodKeyToKey::setPeriodMS(PeriodType::Value type, unsigned int newval)
    {
      if (type == PeriodType::END_) return;

      if (newval == 0) newval = 1;
      periodMS_[type] = newval;
    }

    bool
    DependingPressingPeriodKeyToKey::remap(RemapParams& remapParams)
    {
      bool result = keytokey_[KeyToKeyType::FROM].remap(remapParams);
      if (! result) {
        if (remapParams.params.ex_iskeydown) {
          // another key is pressed.
          dokeydown();
        }
        return false;
      }

      if (remapParams.params.ex_iskeydown) {
        target_ = this;
        active_ = true;
        periodtype_ = PeriodType::NONE;

        savedflags_ = FlagStatus::makeFlags();

        fire_timer_.setTimeoutMS(periodMS_[PeriodType::SHORT_PERIOD]);

      } else {
        dokeydown();
        dokeyup();
      }
      return true;
    }

    void
    DependingPressingPeriodKeyToKey::dokeydown(void)
    {
      if (! active_) return;
      active_ = false;

      fire_timer_.cancelTimeout();

      switch (target_->periodtype_) {
        case PeriodType::NONE:
        {
          target_->periodtype_ = PeriodType::SHORT_PERIOD;
          FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);
          keytokey_[KeyToKeyType::SHORT_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
          break;
        }

        case PeriodType::SHORT_PERIOD:
        case PeriodType::LONG_PERIOD:
        case PeriodType::LONG_LONG_PERIOD:
        case PeriodType::PRESSING_TARGET_KEY_ONLY:
        case PeriodType::END_:
          // do nothing
          break;
      }
    }

    void
    DependingPressingPeriodKeyToKey::dokeyup(void)
    {
      switch (target_->periodtype_) {
        case PeriodType::SHORT_PERIOD:
        {
          target_->periodtype_ = PeriodType::NONE;
          FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);
          keytokey_[KeyToKeyType::SHORT_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          break;
        }
        case PeriodType::LONG_PERIOD:
        {
          target_->periodtype_ = PeriodType::NONE;
          keytokey_[KeyToKeyType::LONG_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          break;
        }
        case PeriodType::LONG_LONG_PERIOD:
        {
          target_->periodtype_ = PeriodType::NONE;
          keytokey_[KeyToKeyType::LONG_LONG_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          break;
        }
        case PeriodType::NONE:
        case PeriodType::PRESSING_TARGET_KEY_ONLY:
        case PeriodType::END_:
          // do nothing
          break;
      }
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

          if (target_->periodMS_[PeriodType::LONG_PERIOD] > 0) {
            fire_timer_.setTimeoutMS(target_->periodMS_[PeriodType::LONG_PERIOD]);
          }
          break;
        }

        case PeriodType::LONG_PERIOD:
        {
          target_->periodtype_ = PeriodType::LONG_LONG_PERIOD;

          (target_->keytokey_[KeyToKeyType::LONG_LONG_PERIOD]).call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
          break;
        }

        case PeriodType::SHORT_PERIOD:
        case PeriodType::LONG_LONG_PERIOD:
        case PeriodType::PRESSING_TARGET_KEY_ONLY:
        case PeriodType::END_:
          // do nothing
          break;
      }
    }
  }
}
