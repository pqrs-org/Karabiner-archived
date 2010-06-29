#include "Config.hpp"
#include "EventWatcher.hpp"
#include "KeyOverlaidModifier.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    TimerWrapper KeyOverlaidModifier::timer_;
    KeyOverlaidModifier* KeyOverlaidModifier::target_ = NULL;

    void
    KeyOverlaidModifier::static_initialize(IOWorkLoop& workloop)
    {
      timer_.initialize(&workloop, NULL, KeyOverlaidModifier::firerepeat);
    }

    void
    KeyOverlaidModifier::static_terminate(void)
    {
      timer_.terminate();
    }

    void
    KeyOverlaidModifier::initialize(void)
    {
      keytokey_.initialize();
      keytokey_fire_.initialize();
      ic_.begin();
    }

    void
    KeyOverlaidModifier::terminate(void)
    {
      keytokey_.terminate();
      keytokey_fire_.terminate();
    }

    void
    KeyOverlaidModifier::add(KeyCode newval)
    {
      switch (index_) {
        case 0:
          keytokey_.add(newval);
          keytokey_fire_.add(KeyCode::VK_PSEUDO_KEY);
          break;

        case 1:
          toKey_.key = newval;
          keytokey_.add(newval);
          break;

        default:
          keytokey_fire_.add(newval);
          break;
      }
      ++index_;
    }

    void
    KeyOverlaidModifier::add(Flags newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid KeyOverlaidModifier::add\n");
          break;

        case 1:
          keytokey_.add(newval);
          break;

        case 2:
          toKey_.flags = newval;
          keytokey_.add(newval);
          break;

        default:
          keytokey_fire_.add(newval);
          break;
      }
    }

    void
    KeyOverlaidModifier::add(KeyOverlaidModifier::Option newval)
    {
      switch (newval) {
        case KeyOverlaidModifier::OPTION_REPEAT:
          isRepeatEnabled_ = true;
          break;
      }
    }

    bool
    KeyOverlaidModifier::remap(RemapParams& remapParams)
    {
      bool isKeyDown = remapParams.isKeyDownOrModifierDown();
      bool savedIsAnyEventHappen = isAnyEventHappen_;

      bool result = keytokey_.remap(remapParams);
      if (! result) return false;

      // ------------------------------------------------------------
      IOLockWrapper::ScopedLock lk(timer_.getlock());

      if (isKeyDown) {
        EventWatcher::set(isAnyEventHappen_);
        ic_.begin();

        // ----------------------------------------
        // We store the flags when KeyDown.
        // Because it lets you make a natural input when the following sequence.
        //
        // ex. "Space to Shift (when type only, send Space)"
        // (1) KeyDown Command_L
        // (2) KeyDown Space        save flags (Command_L)
        // (3) KeyUp   Command_L
        // (4) KeyUp   Space        fire Command_L+Space

        FlagStatus::temporary_decrease(toKey_.flags | toKey_.key.getModifierFlag());
        savedflags_ = FlagStatus::makeFlags();
        FlagStatus::temporary_increase(toKey_.flags | toKey_.key.getModifierFlag());

        // ----------------------------------------
        if (isRepeatEnabled_) {
          target_ = this;
          isfirenormal_ = false;
          isfirerepeat_ = false;
          timer_.setTimeoutMS(config.get_keyoverlaidmodifier_initial_wait());
        }

      } else {
        if (isfirerepeat_) {
          keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);

        } else {
          isfirenormal_ = true;

          if (savedIsAnyEventHappen == false) {
            if (config.parameter_keyoverlaidmodifier_timeout <= 0 || ic_.checkThreshold(config.parameter_keyoverlaidmodifier_timeout) == false) {
              {
                FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);

                keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
                keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);
              }
            }
          }
          EventWatcher::unset(isAnyEventHappen_);
        }
      }

      return true;
    }

    void
    KeyOverlaidModifier::firerepeat(OSObject* owner, IOTimerEventSource* sender)
    {
      IOLockWrapper::ScopedLock lk(timer_.getlock());

      if (! target_) return;

      if (! target_->isfirenormal_) {
        target_->isfirerepeat_ = true;

        (target_->keytokey_fire_).call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
      }
    }
  }
}
