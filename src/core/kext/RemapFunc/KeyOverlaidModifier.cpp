#include "Config.hpp"
#include "EventWatcher.hpp"
#include "KeyOverlaidModifier.hpp"
#include "../VirtualKey/VK_LAZY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    TimerWrapper KeyOverlaidModifier::firerepeat_timer_;
    KeyOverlaidModifier* KeyOverlaidModifier::target_ = NULL;

    void
    KeyOverlaidModifier::static_initialize(IOWorkLoop& workloop)
    {
      firerepeat_timer_.initialize(&workloop, NULL, KeyOverlaidModifier::firerepeat_timer_callback);
    }

    void
    KeyOverlaidModifier::static_terminate(void)
    {
      firerepeat_timer_.terminate();
    }

    KeyOverlaidModifier::KeyOverlaidModifier(void) :
      index_(0), isAnyEventHappen_(false), savedflags_(0), isRepeatEnabled_(false)
    {
      ic_.begin();
    }

    KeyOverlaidModifier::~KeyOverlaidModifier(void)
    {
      if (target_ == this) {
        firerepeat_timer_.cancelTimeout();
        target_ = NULL;
      }
    }

    void
    KeyOverlaidModifier::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              keytokey_.add(KeyCode(newval));
              keytokey_fire_.add(KeyCode::VK_PSEUDO_KEY);
              break;

            case 1:
              toKey_.key = newval;
              keytokey_.add(KeyCode(newval));
              break;

            default:
              keytokey_fire_.add(KeyCode(newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid KeyOverlaidModifier::add\n");
              break;

            case 1:
              keytokey_.add(Flags(newval));
              break;

            case 2:
              toKey_.flags = newval;
              keytokey_.add(Flags(newval));
              break;

            default:
              keytokey_fire_.add(Flags(newval));
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          if (Option::KEYOVERLAIDMODIFIER_REPEAT == newval) {
            isRepeatEnabled_ = true;
          } else {
            IOLOG_ERROR("KeyOverlaidModifier::add unknown option:%d\n", newval);
          }
          break;
        }

        default:
          IOLOG_ERROR("KeyOverlaidModifier::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    KeyOverlaidModifier::remap(RemapParams& remapParams)
    {
      bool savedIsAnyEventHappen = isAnyEventHappen_;

      bool result = keytokey_.remap(remapParams);
      if (! result) return false;

      // ------------------------------------------------------------
      if (remapParams.params.ex_iskeydown) {
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

        FlagStatus::temporary_decrease(toKey_.flags | toKey_.key.getModifierFlag() | VirtualKey::VK_LAZY::getModifierFlag(toKey_.key));
        savedflags_ = FlagStatus::makeFlags();
        FlagStatus::temporary_increase(toKey_.flags | toKey_.key.getModifierFlag() | VirtualKey::VK_LAZY::getModifierFlag(toKey_.key));

        // ----------------------------------------
        if (isRepeatEnabled_) {
          target_ = this;
          isfirenormal_ = false;
          isfirerepeat_ = false;
          firerepeat_timer_.setTimeoutMS(Config::get_keyoverlaidmodifier_initial_wait());
        }

      } else {
        firerepeat_timer_.cancelTimeout();

        if (isfirerepeat_) {
          FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);
          keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);

        } else {
          isfirenormal_ = true;

          if (savedIsAnyEventHappen == false) {
            int timeout = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_keyoverlaidmodifier_timeout);
            if (timeout <= 0 || ic_.checkThreshold(timeout) == false) {
              FlagStatus::ScopedTemporaryFlagsChanger stfc(savedflags_);

              keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
              keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);
            }
          }
          EventWatcher::unset(isAnyEventHappen_);
        }
      }

      return true;
    }

    void
    KeyOverlaidModifier::firerepeat_timer_callback(OSObject* owner, IOTimerEventSource* sender)
    {
      if (! target_) return;

      if (target_->isAnyEventHappen_) return;

      if (! target_->isfirenormal_) {
        target_->isfirerepeat_ = true;

        FlagStatus::ScopedTemporaryFlagsChanger stfc(target_->savedflags_);
        (target_->keytokey_fire_).call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
      }
    }
  }
}
