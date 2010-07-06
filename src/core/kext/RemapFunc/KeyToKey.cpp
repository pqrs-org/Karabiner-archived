#include "CommonData.hpp"
#include "EventOutputQueue.hpp"
#include "KeyToKey.hpp"
#include "KeyboardRepeat.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    KeyToKey::initialize(void)
    {
      toKeys_ = new Vector_PairKeyFlags();
    }

    void
    KeyToKey::terminate(void)
    {
      if (toKeys_) {
        delete toKeys_;
        toKeys_ = NULL;
      }
    }

    void
    KeyToKey::add(KeyCode newval)
    {
      if (! toKeys_) return;

      switch (index_) {
        case 0:
          fromKey_.key = newval;
          break;
        default:
          toKeys_->push_back(PairKeyFlags(newval));
          break;
      }
      ++index_;
    }

    void
    KeyToKey::add(Flags newval)
    {
      if (! toKeys_) return;

      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid KeyToKey::add\n");
          break;
        case 1:
          fromKey_.flags = newval;
          break;
        default:
          (toKeys_->back()).flags = newval;
          break;
      }
    }

    bool
    KeyToKey::remap(RemapParams& remapParams)
    {
      if (! toKeys_) return false;

      if (remapParams.isremapped) return false;
      if (! fromkeychecker_.isFromKey(remapParams.params, fromKey_.key, fromKey_.flags)) return false;
      remapParams.isremapped = true;

      // ------------------------------------------------------------
      // handle EventType & Modifiers
      bool isKeyDown = remapParams.isKeyDownOrModifierDown();
      if (isKeyDown) {
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
          EventType newEventType = isKeyDown ? EventType::DOWN : EventType::UP;
          ModifierFlag toModifierFlag = (*toKeys_)[0].key.getModifierFlag();

          if (toModifierFlag == ModifierFlag::NONE) {
            // toKey
            FlagStatus::temporary_decrease(fromKey_.flags);
            FlagStatus::temporary_increase((*toKeys_)[0].flags);

          } else {
            // toModifier
            newEventType = EventType::MODIFY;

            if (isKeyDown) {
              FlagStatus::increase((*toKeys_)[0].flags | toModifierFlag);
              FlagStatus::decrease(fromKey_.flags);
            } else {
              FlagStatus::decrease((*toKeys_)[0].flags | toModifierFlag);
              FlagStatus::increase(fromKey_.flags);
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
          if (isKeyDown) {
            FlagStatus::temporary_decrease(fromKey_.flags);

            for (size_t i = 0; i < toKeys_->size(); ++i) {
              FlagStatus::temporary_increase((*toKeys_)[i].flags);

              Flags f = FlagStatus::makeFlags();
              KeyboardType keyboardType = remapParams.params.keyboardType;

              EventOutputQueue::FireKey::fire_downup(f, (*toKeys_)[i].key, keyboardType);
              KeyboardRepeat::primitive_add(EventType::DOWN, f, (*toKeys_)[i].key, keyboardType);
              KeyboardRepeat::primitive_add(EventType::UP,   f, (*toKeys_)[i].key, keyboardType);

              FlagStatus::temporary_decrease((*toKeys_)[i].flags);
            }

            KeyboardRepeat::primitive_start();
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
