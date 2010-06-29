#include "CommonData.hpp"
#include "Config.hpp"
#include "HoldingKeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    TimerWrapper HoldingKeyToKey::timer_;
    HoldingKeyToKey* HoldingKeyToKey::target_ = NULL;

    void
    HoldingKeyToKey::static_initialize(IOWorkLoop& workloop)
    {
      timer_.initialize(&workloop, NULL, HoldingKeyToKey::fireholding);
    }

    void
    HoldingKeyToKey::static_terminate(void)
    {
      timer_.terminate();
    }

    void
    HoldingKeyToKey::initialize(void)
    {
      keytokey_drop_.initialize();
      keytokey_normal_.initialize();
      keytokey_holding_.initialize();
    }

    void
    HoldingKeyToKey::terminate(void)
    {
      keytokey_drop_.terminate();
      keytokey_normal_.terminate();
      keytokey_holding_.terminate();
    }

    void
    HoldingKeyToKey::add(KeyCode newval)
    {
      switch (index_) {
        case 0:
          fromKey_.key = newval;
          keytokey_drop_.add(newval);
          keytokey_normal_.add(KeyCode::VK_PSEUDO_KEY);
          keytokey_holding_.add(KeyCode::VK_PSEUDO_KEY);
          break;

        default:
          if (newval == KeyCode::VK_NONE) {
            index_is_holding_ = true;
          } else {
            if (index_is_holding_) {
              keytokey_holding_.add(newval);
            } else {
              keytokey_normal_.add(newval);
            }
          }
          break;
      }
      ++index_;
    }

    void
    HoldingKeyToKey::add(Flags newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid HoldingKeyToKey::add\n");
          break;

        case 1:
          fromKey_.flags = newval;
          keytokey_drop_.add(newval);
          break;

        default:
          if (index_is_holding_) {
            keytokey_holding_.add(newval);
          } else {
            keytokey_normal_.add(newval);
          }
          break;
      }
    }

    bool
    HoldingKeyToKey::remap(RemapParams& remapParams)
    {
      bool isKeyDown = remapParams.isKeyDownOrModifierDown();

      bool result = keytokey_drop_.remap(remapParams);
      if (! result) return false;

      // ------------------------------------------------------------
      IOLockWrapper::ScopedLock lk(timer_.getlock());

      if (isKeyDown) {
        target_ = this;
        isfirenormal_ = false;
        isfireholding_ = false;

        FlagStatus::temporary_decrease(fromKey_.flags | fromKey_.key.getModifierFlag());
        savedflags_ = FlagStatus::makeFlags();
        FlagStatus::temporary_increase(fromKey_.flags | fromKey_.key.getModifierFlag());

        timer_.setTimeoutMS(config.get_holdingkeytokey_wait());

      } else {
        timer_.cancelTimeout();
        if (! isfireholding_) {
          isfirenormal_ = true;

          {
            FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);

            keytokey_normal_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
            keytokey_normal_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          }
        }
      }
      return true;
    }

    void
    HoldingKeyToKey::fireholding(OSObject* owner, IOTimerEventSource* sender)
    {
      IOLockWrapper::ScopedLock lk(timer_.getlock());

      if (! target_) return;

      if (! target_->isfirenormal_) {
        target_->isfireholding_ = true;

        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::DOWN,
                                                                                       FlagStatus::makeFlags(),
                                                                                       KeyCode::VK_PSEUDO_KEY,
                                                                                       CommonData::getcurrent_keyboardType(),
                                                                                       false));
        if (! ptr) return;
        Params_KeyboardEventCallBack& params = *ptr;

        RemapParams rp_down(params);
        target_->keytokey_holding_.remap(rp_down);

        params.eventType = EventType::UP;
        RemapParams rp_up(params);
        target_->keytokey_holding_.remap(rp_up);
      }
    }
  }
}
