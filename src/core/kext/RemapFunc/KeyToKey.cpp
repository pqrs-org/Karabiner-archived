#include "CommonData.hpp"
#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "KeyToKey.hpp"
#include "KeyboardRepeat.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    KeyToKey::KeyToKey(void) :
      index_(0),
      currentVectorPointer_(&toKeys_),
      keyboardRepeatID_(-1),
      isRepeatEnabled_(true),
      delayUntilRepeat_(-1),
      keyRepeat_(-1)
    {}

    KeyToKey::~KeyToKey(void)
    {}

    void
    KeyToKey::add(unsigned int datatype, unsigned int newval)
    {
      if (! currentVectorPointer_) { return; }

      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          switch (index_) {
            case 0:
              fromEvent_ = FromEvent(datatype, newval);
              break;
            default:
              currentVectorPointer_->push_back(ToEvent(datatype, newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid KeyToKey::add\n");
              break;
            case 1:
              fromFlags_ = Flags(newval);
              break;
            default:
              if (! currentVectorPointer_->empty()) {
                (currentVectorPointer_->back()).setFlags(Flags(newval));
              }
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          Option option(newval);
          if (Option::NOREPEAT == option) {
            isRepeatEnabled_ = false;
          } else if (Option::KEYTOKEY_BEFORE_KEYDOWN == option) {
            currentVectorPointer_ = &beforeKeys_;
          } else if (Option::KEYTOKEY_AFTER_KEYUP == option) {
            currentVectorPointer_ = &afterKeys_;
          } else if (Option::USE_SEPARATOR == option ||
                     Option::SEPARATOR == option) {
            // do nothing
          } else {
            IOLOG_ERROR("KeyToKey::add unknown option:%d\n", newval);
          }
          break;
        }

        case BRIDGE_DATATYPE_DELAYUNTILREPEAT:
        {
          delayUntilRepeat_ = newval;
          break;
        }

        case BRIDGE_DATATYPE_KEYREPEAT:
        {
          keyRepeat_ = newval;
          break;
        }

        default:
          IOLOG_ERROR("KeyToKey::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    KeyToKey::remap(RemapParams& remapParams)
    {
      if (remapParams.isremapped) return false;
      if (! fromEvent_.changePressingState(remapParams.paramsUnion, FlagStatus::makeFlags(), fromFlags_)) return false;
      remapParams.isremapped = true;

      // ------------------------------------------------------------
      // handle EventType & Modifiers

      // Let's consider the following setting.
      //   __KeyToKey__ KeyCode::SHIFT_R, ModifierFlag::SHIFT_R | ModifierFlag::NONE, KeyCode::A, ModifierFlag::SHIFT_R
      // In this setting, we need decrease SHIFT_R only once.
      // So, we transform values of fromFlags_.
      //
      // [before]
      //   fromEvent_ : KeyCode::SHIFT_R
      //   fromFlags_ : ModifierFlag::SHIFT_R | ModifierFlag::NONE
      //
      // [after]
      //   fromEvent_ : KeyCode::SHIFT_R
      //   fromFlags_ : ModifierFlag::NONE
      //
      // Note: we need to apply this transformation after calling fromEvent_.changePressingState.

      Flags fromFlags = fromFlags_;
      fromFlags.remove(fromEvent_.getModifierFlag());

      if (fromEvent_.isPressing()) {
        FlagStatus::decrease(fromEvent_.getModifierFlag());
        ButtonStatus::decrease(fromEvent_.getPointingButton());
      } else {
        FlagStatus::increase(fromEvent_.getModifierFlag());
        ButtonStatus::increase(fromEvent_.getPointingButton());
      }

      // ----------------------------------------
      // Handle beforeKeys_
      if (fromEvent_.isPressing()) {
        FlagStatus::temporary_decrease(fromFlags);

        for (size_t i = 0; i < beforeKeys_.size(); ++i) {
          FlagStatus::temporary_increase(beforeKeys_[i].getFlags());

          Flags f = FlagStatus::makeFlags();
          beforeKeys_[i].fire_downup(f);

          FlagStatus::temporary_decrease(beforeKeys_[i].getFlags());
        }

        FlagStatus::temporary_increase(fromFlags);
      }

      // ----------------------------------------
      // Handle toKeys_
      bool add_to_keyrepeat = true;
      if (fromEvent_.isPressing() && ! isRepeatEnabled_) {
        add_to_keyrepeat = false;
      }

      switch (toKeys_.size()) {
        case 0:
          break;

        case 1:
        {
          EventType newEventType = fromEvent_.isPressing() ? EventType::DOWN : EventType::UP;
          ModifierFlag toModifierFlag = toKeys_[0].getModifierFlag();

          if (toModifierFlag == ModifierFlag::NONE && ! toKeys_[0].isEventLikeModifier()) {
            // toKey
            FlagStatus::temporary_decrease(fromFlags);
            FlagStatus::temporary_increase(toKeys_[0].getFlags());

          } else {
            // toModifier or VirtualKey::isKeyLikeModifier
            if (toModifierFlag != ModifierFlag::NONE) {
              newEventType = EventType::MODIFY;
            }

            if (fromEvent_.isPressing()) {
              FlagStatus::increase(toKeys_[0].getFlags() | toModifierFlag);
              FlagStatus::decrease(fromFlags);
            } else {
              FlagStatus::decrease(toKeys_[0].getFlags() | toModifierFlag);
              FlagStatus::increase(fromFlags);
            }
          }

          toKeys_[0].fire(newEventType, FlagStatus::makeFlags(),
                          add_to_keyrepeat, getDelayUntilRepeat(), getKeyRepeat());

          if (! add_to_keyrepeat) {
            KeyboardRepeat::cancel();
          }

          break;
        }

        default:
          ToEvent& lastToEvent                 = toKeys_[toKeys_.size() - 1];
          ModifierFlag lastToEventModifierFlag = lastToEvent.getModifierFlag();
          bool isLastToEventModifierKey        = (lastToEventModifierFlag != ModifierFlag::NONE);
          bool isLastToEventLikeModifier       = lastToEvent.isEventLikeModifier();

          if (fromEvent_.isPressing()) {
            KeyboardRepeat::cancel();

            FlagStatus::temporary_decrease(fromFlags);

            size_t size = toKeys_.size();
            // If the last key is modifier, we give it special treatment.
            // - Don't fire key repeat.
            // - Synchronous the key press status and the last modifier status.
            if (isLastToEventModifierKey || isLastToEventLikeModifier) {
              --size;
            }

            for (size_t i = 0; i < size; ++i) {
              FlagStatus::temporary_increase(toKeys_[i].getFlags());

              Flags f = FlagStatus::makeFlags();

              toKeys_[i].fire_downup(f, true);

              FlagStatus::temporary_decrease(toKeys_[i].getFlags());
            }

            if (isLastToEventModifierKey || isLastToEventLikeModifier) {
              // restore temporary flag.
              FlagStatus::temporary_increase(fromFlags);

              FlagStatus::increase(lastToEvent.getFlags() | lastToEventModifierFlag);
              FlagStatus::decrease(fromFlags);

              if (isLastToEventLikeModifier) {
                // Don't call EventOutputQueue::FireModifiers::fire here.
                //
                // Intentionally VK_LAZY_* stop sending MODIFY events.
                // EventOutputQueue::FireModifiers::fire destroys this behavior.
                lastToEvent.fire(EventType::DOWN, FlagStatus::makeFlags(), false);

              } else {
                EventOutputQueue::FireModifiers::fire();
              }
            }

            if (isLastToEventModifierKey || isLastToEventLikeModifier) {
              KeyboardRepeat::cancel();
            } else {
              if (isRepeatEnabled_) {
                keyboardRepeatID_ = KeyboardRepeat::primitive_start(getDelayUntilRepeat(), getKeyRepeat());
              } else {
                keyboardRepeatID_ = -1;
              }
            }

          } else {
            if (isLastToEventModifierKey || isLastToEventLikeModifier) {
              // For Lazy-Modifiers (KeyCode::VK_LAZY_*),
              // we need to handle these keys before restoring fromFlags, lastKeyFlags and lastKeyModifierFlag.
              // The unnecessary modifier events occur unless we do it.
              if (isLastToEventLikeModifier) {
                lastToEvent.fire(EventType::UP, FlagStatus::makeFlags(), false);
              }

              FlagStatus::decrease(lastToEvent.getFlags() | lastToEventModifierFlag);
              FlagStatus::increase(fromFlags);
              EventOutputQueue::FireModifiers::fire();

            } else {
              if (KeyboardRepeat::getID() == keyboardRepeatID_) {
                KeyboardRepeat::cancel();
              }
            }
          }
          break;
      }

      // ----------------------------------------
      // Handle afterKeys_
      if (! fromEvent_.isPressing()) {
        // We need to keep temporary flags for "general.lazy_modifiers_with_mouse_event" when afterKeys_ is empty.
        if (afterKeys_.size() > 0) {
          // clear temporary flags.
          FlagStatus::set();

          FlagStatus::temporary_decrease(fromFlags);

          for (size_t i = 0; i < afterKeys_.size(); ++i) {
            FlagStatus::temporary_increase(afterKeys_[i].getFlags());

            Flags f = FlagStatus::makeFlags();
            afterKeys_[i].fire_downup(f);

            FlagStatus::temporary_decrease(afterKeys_[i].getFlags());
          }

          FlagStatus::temporary_increase(fromFlags);
        }
      }

      return true;
    }

    bool
    KeyToKey::call_remap_with_VK_PSEUDO_KEY(EventType eventType)
    {
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(eventType,
                                                                                     FlagStatus::makeFlags(),
                                                                                     KeyCode::VK_PSEUDO_KEY,
                                                                                     CommonData::getcurrent_keyboardType(),
                                                                                     false));
      if (! ptr) return false;
      Params_KeyboardEventCallBack& params = *ptr;

      ParamsUnion paramsUnion(params);
      RemapParams rp(paramsUnion);
      return remap(rp);
    }

    int
    KeyToKey::getDelayUntilRepeat(void)
    {
      if (delayUntilRepeat_ >= 0) {
        return delayUntilRepeat_;
      } else {
        // If all ToEvent is consumer, use repeat.consumer_initial_wait.
        for (size_t i = 0; i < toKeys_.size(); ++i) {
          if (toKeys_[i].getType() != ToEvent::Type::CONSUMER_KEY) {
            return Config::get_repeat_initial_wait();
          }
        }

        return Config::get_repeat_consumer_initial_wait();
      }
    }

    int
    KeyToKey::getKeyRepeat(void)
    {
      if (keyRepeat_ >= 0) {
        return keyRepeat_;
      } else {
        // If all ToEvent is consumer, use repeat.consumer_wait.
        for (size_t i = 0; i < toKeys_.size(); ++i) {
          if (toKeys_[i].getType() != ToEvent::Type::CONSUMER_KEY) {
            return Config::get_repeat_wait();
          }
        }

        return Config::get_repeat_wait();
      }
    }
  }
}
