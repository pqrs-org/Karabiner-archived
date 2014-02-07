#include "CommonData.hpp"
#include "EventOutputQueue.hpp"
#include "KeyboardRepeat.hpp"
#include "ToEvent.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  ToEvent::isEventLikeModifier(void) const
  {
    switch (type_) {
      case Type::NONE:            return false;
      case Type::KEY:             return VirtualKey::isKeyLikeModifier(key_);
      case Type::CONSUMER_KEY:    return false;
      case Type::POINTING_BUTTON: return true;
    }
  }

  void
  ToEvent::fire(EventType eventType, Flags flags,
                bool add_to_keyrepeat, int delayUntilRepeat, int keyRepeat)
  {
    switch (type_) {
      case Type::NONE:
        break;

      case Type::KEY:
      {
        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(eventType,
                                                                                       flags,
                                                                                       key_,
                                                                                       CommonData::getcurrent_keyboardType(),
                                                                                       false));
        if (ptr) {
          EventOutputQueue::FireKey::fire(*ptr);
          if (add_to_keyrepeat) {
            KeyboardRepeat::set(*ptr, delayUntilRepeat, keyRepeat);
          }
        }
        break;
      }

      case Type::CONSUMER_KEY:
      {
        Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(eventType,
                                                                                                     flags,
                                                                                                     consumer_,
                                                                                                     false));
        if (ptr) {
          EventOutputQueue::FireConsumer::fire(*ptr);
          if (add_to_keyrepeat) {
            KeyboardRepeat::set(*ptr, delayUntilRepeat, keyRepeat);
          }
        }
        break;
      }

      case Type::POINTING_BUTTON:
      {
        if (eventType == EventType::DOWN) {
          ButtonStatus::increase(button_);
        } else {
          ButtonStatus::decrease(button_);
        }

        EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());

        if (add_to_keyrepeat) {
          // Do nothing
        }
        break;
      }
    }
  }

  void
  ToEvent::fire_downup(Flags flags, bool add_to_keyrepeat)
  {
    switch (type_) {
      case Type::NONE:
        break;

      case Type::KEY:
      {
        KeyboardType keyboardType = CommonData::getcurrent_keyboardType();

        EventOutputQueue::FireKey::fire_downup(flags, key_, keyboardType);
        if (add_to_keyrepeat) {
          KeyboardRepeat::primitive_add_downup(flags, key_, keyboardType);
        }
        break;
      }

      case Type::CONSUMER_KEY:
      {
        EventOutputQueue::FireConsumer::fire_downup(flags, consumer_);
        if (add_to_keyrepeat) {
          KeyboardRepeat::primitive_add(EventType::DOWN, flags, consumer_);
          KeyboardRepeat::primitive_add(EventType::UP,   flags, consumer_);
        }
        break;
      }

      case Type::POINTING_BUTTON:
      {
        FlagStatus::ScopedTemporaryFlagsChanger stfc(flags);

        for (int i = 0; i < 2; ++i) {
          if (i == 0) {
            ButtonStatus::increase(button_);
          } else {
            ButtonStatus::decrease(button_);
          }

          Buttons buttons(ButtonStatus::makeButtons());
          EventOutputQueue::FireRelativePointer::fire(buttons);
          if (add_to_keyrepeat) {
            KeyboardRepeat::primitive_add(buttons);
          }
        }
        break;
      }
    }
  }
}
