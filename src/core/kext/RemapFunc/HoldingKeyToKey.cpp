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

    HoldingKeyToKey::HoldingKeyToKey(void) : index_(0), index_is_holding_(false)
    {}

    HoldingKeyToKey::~HoldingKeyToKey(void)
    {}

    void
    HoldingKeyToKey::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              keytokey_drop_.add(KeyCode(newval));
              keytokey_normal_.add(KeyCode::VK_PSEUDO_KEY);
              keytokey_holding_.add(KeyCode::VK_PSEUDO_KEY);
              break;

            case 1:
              // pass-through (== no break)
              keytokey_drop_.add(KeyCode::VK_NONE);
            default:
              if (KeyCode::VK_NONE == newval) {
                index_is_holding_ = true;
              } else {
                if (index_is_holding_) {
                  keytokey_holding_.add(KeyCode(newval));
                } else {
                  keytokey_normal_.add(KeyCode(newval));
                }
              }
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid HoldingKeyToKey::add\n");
              break;

            case 1:
              keytokey_drop_.add(Flags(newval));
              break;

            default:
              if (index_is_holding_) {
                keytokey_holding_.add(Flags(newval));
              } else {
                keytokey_normal_.add(Flags(newval));
              }
              break;
          }
          break;
        }

        default:
          IOLOG_ERROR("HoldingKeyToKey::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    HoldingKeyToKey::remap(RemapParams& remapParams)
    {
      bool result = keytokey_drop_.remap(remapParams);
      if (! result) return false;

      // ------------------------------------------------------------
      IOLockWrapper::ScopedLock lk(timer_.getlock());

      if (remapParams.params.ex_iskeydown) {
        target_ = this;
        isfirenormal_ = false;
        isfireholding_ = false;

        savedflags_ = FlagStatus::makeFlags();

        timer_.setTimeoutMS(config.get_holdingkeytokey_wait());

      } else {
        timer_.cancelTimeout();

        if (isfireholding_) {
          keytokey_holding_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);

        } else {
          isfirenormal_ = true;

          FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);
          keytokey_normal_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
          keytokey_normal_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);
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

        (target_->keytokey_holding_).call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
      }
    }
  }
}
