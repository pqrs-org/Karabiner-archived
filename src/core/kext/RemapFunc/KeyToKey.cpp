#include "CommonData.hpp"
#include "EventOutputQueue.hpp"
#include "KeyToKey.hpp"
#include "KeyboardRepeat.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    KeyToKey::KeyToKey(void) : index_(0)
    {
      toKeys_ = new Vector_PairKeyFlags();
    }

    KeyToKey::~KeyToKey(void)
    {
      if (toKeys_) {
        delete toKeys_;
      }
    }

    void
    KeyToKey::add(unsigned int datatype, unsigned int newval)
    {
      if (! toKeys_) return;

      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              fromKey_.key = newval;
              break;
            default:
              toKeys_->push_back(PairKeyFlags(newval));
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
              fromKey_.flags = newval;
              break;
            default:
              if (! toKeys_->empty()) {
                (toKeys_->back()).flags = newval;
              }
              break;
          }
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
      if (! toKeys_) return false;

      if (remapParams.isremapped) return false;
      if (! fromkeychecker_.isFromKey(remapParams.params.ex_iskeydown, remapParams.params.key, FlagStatus::makeFlags(), fromKey_.key, fromKey_.flags)) return false;
      remapParams.isremapped = true;

      // ------------------------------------------------------------
      // handle EventType & Modifiers

      // Let's consider the following setting.
      //   --KeyToKey-- KeyCode::SHIFT_R, ModifierFlag::SHIFT_R | ModifierFlag::NONE, KeyCode::A, ModifierFlag::SHIFT_R
      // In this setting, we need decrease SHIFT_R only once.
      // So, we transform values of fromKey_.
      //
      // [before]
      //   fromKey_.key   : KeyCode::SHIFT_R
      //   fromKey_.flags : ModifierFlag::SHIFT_R | ModifierFlag::NONE
      //
      // [after]
      //   fromKey_.key   : KeyCode::SHIFT_R
      //   fromKey_.flags : ModifierFlag::NONE
      //
      // Note: we need to apply this transformation after calling fromkeychecker_.isFromKey.

      Flags fromFlags = fromKey_.flags;
      fromFlags.remove(fromKey_.key.getModifierFlag());

      if (remapParams.params.ex_iskeydown) {
        FlagStatus::decrease(fromKey_.key.getModifierFlag());
      } else {
        FlagStatus::increase(fromKey_.key.getModifierFlag());
      }
      KeyboardRepeat::cancel();

      switch (toKeys_->size()) {
        case 0:
          break;

        case 1:
        {
          EventType newEventType = remapParams.params.ex_iskeydown ? EventType::DOWN : EventType::UP;
          ModifierFlag toModifierFlag = (*toKeys_)[0].key.getModifierFlag();

          if (toModifierFlag == ModifierFlag::NONE) {
            // toKey
            FlagStatus::temporary_decrease(fromFlags);
            FlagStatus::temporary_increase((*toKeys_)[0].flags);

          } else {
            // toModifier
            newEventType = EventType::MODIFY;

            if (remapParams.params.ex_iskeydown) {
              FlagStatus::increase((*toKeys_)[0].flags | toModifierFlag);
              FlagStatus::decrease(fromFlags);
            } else {
              FlagStatus::decrease((*toKeys_)[0].flags | toModifierFlag);
              FlagStatus::increase(fromFlags);
            }
          }

          // ----------------------------------------
          Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(newEventType,
                                                                                         FlagStatus::makeFlags(),
                                                                                         (*toKeys_)[0].key,
                                                                                         remapParams.params.keyboardType,
                                                                                         remapParams.params.repeat));
          if (! ptr) return false;
          Params_KeyboardEventCallBack& params = *ptr;

          KeyboardRepeat::set(params);
          EventOutputQueue::FireKey::fire(params);

          break;
        }

        default:
          ModifierFlag lastKeyModifierFlag = (*toKeys_)[toKeys_->size() - 1].key.getModifierFlag();
          Flags        lastKeyFlags        = (*toKeys_)[toKeys_->size() - 1].flags;
          bool         isLastKeyModifier   = (lastKeyModifierFlag != ModifierFlag::NONE);

          if (remapParams.params.ex_iskeydown) {
            FlagStatus::temporary_decrease(fromFlags);

            size_t size = toKeys_->size();
            // If the last key is modifier, we give it special treatment.
            // - Don't fire key repeat.
            // - Synchronous the key press status and the last modifier status.
            if (isLastKeyModifier) {
              --size;
            }

            for (size_t i = 0; i < size; ++i) {
              FlagStatus::temporary_increase((*toKeys_)[i].flags);

              Flags f = FlagStatus::makeFlags();
              KeyboardType keyboardType = remapParams.params.keyboardType;

              EventOutputQueue::FireKey::fire_downup(f, (*toKeys_)[i].key, keyboardType);
              KeyboardRepeat::primitive_add_downup(f, (*toKeys_)[i].key, keyboardType);

              FlagStatus::temporary_decrease((*toKeys_)[i].flags);
            }

            if (isLastKeyModifier) {
              // restore temporary flag.
              FlagStatus::temporary_increase(fromFlags);

              FlagStatus::increase(lastKeyFlags | lastKeyModifierFlag);
              FlagStatus::decrease(fromFlags);
              EventOutputQueue::FireModifiers::fire();
            }

            if (isLastKeyModifier) {
              KeyboardRepeat::cancel();
            } else {
              KeyboardRepeat::primitive_start();
            }

          } else {
            if (isLastKeyModifier) {
              FlagStatus::decrease(lastKeyFlags | lastKeyModifierFlag);
              FlagStatus::increase(fromFlags);
              EventOutputQueue::FireModifiers::fire();
            }
          }
          break;
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

      RemapParams rp(params);
      return remap(rp);
    }
  }
}
