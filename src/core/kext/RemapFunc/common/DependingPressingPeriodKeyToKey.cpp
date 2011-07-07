#include "Config.hpp"
#include "DependingPressingPeriodKeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    TimerWrapper DependingPressingPeriodKeyToKey::fireholding_timer_;
    DependingPressingPeriodKeyToKey* DependingPressingPeriodKeyToKey::target_ = NULL;

    void
    DependingPressingPeriodKeyToKey::static_initialize(IOWorkLoop& workloop)
    {
      fireholding_timer_.initialize(&workloop, NULL, DependingPressingPeriodKeyToKey::fireholding_timer_callback);
    }

    void
    DependingPressingPeriodKeyToKey::static_terminate(void)
    {
      fireholding_timer_.terminate();
    }

    DependingPressingPeriodKeyToKey::DependingPressingPeriodKeyToKey(void) : index_(0), index_is_holding_(false), active_(false), keydowntype_(KEYDOWNTYPE_NONE)
    {}

    DependingPressingPeriodKeyToKey::~DependingPressingPeriodKeyToKey(void)
    {
      if (target_ == this) {
        fireholding_timer_.cancelTimeout();
        target_ = NULL;
      }
    }

    void
    DependingPressingPeriodKeyToKey::add(KeyToKeyType::Value type, unsigned int datatype, unsigned int newval)
    {
      if (type == KeyToKeyType::END_) return;
      keytokey_[type].add(datatype, newval);
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
        keydowntype_ = KEYDOWNTYPE_NONE;

        savedflags_ = FlagStatus::makeFlags();

        fireholding_timer_.setTimeoutMS(Config::get_holdingkeytokey_wait());

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

      fireholding_timer_.cancelTimeout();

      switch (target_->keydowntype_) {
        case KEYDOWNTYPE_NONE:
        {
          target_->keydowntype_ = KEYDOWNTYPE_NORMAL;
          FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);
          keytokey_[KeyToKeyType::SHORT_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
          break;
        }

        case KEYDOWNTYPE_NORMAL:
        case KEYDOWNTYPE_HOLDING:
          // do nothing
          break;
      }
    }

    void
    DependingPressingPeriodKeyToKey::dokeyup(void)
    {
      switch (target_->keydowntype_) {
        case KEYDOWNTYPE_NORMAL:
        {
          target_->keydowntype_ = KEYDOWNTYPE_NONE;
          FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);
          keytokey_[KeyToKeyType::SHORT_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          break;
        }
        case KEYDOWNTYPE_HOLDING:
        {
          target_->keydowntype_ = KEYDOWNTYPE_NONE;
          keytokey_[KeyToKeyType::LONG_PERIOD].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          break;
        }
        case KEYDOWNTYPE_NONE:
          // do nothing
          break;
      }
    }

    void
    DependingPressingPeriodKeyToKey::fireholding_timer_callback(OSObject* owner, IOTimerEventSource* sender)
    {
      if (! target_) return;

      if (target_->keydowntype_ == KEYDOWNTYPE_NONE) {
        target_->keydowntype_ = KEYDOWNTYPE_HOLDING;

        (target_->keytokey_[KeyToKeyType::LONG_PERIOD]).call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
      }
    }
  }
}
