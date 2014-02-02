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

        ButtonStatus::increase(button_);
        EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());
        ButtonStatus::decrease(button_);
        EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());

        // XXX: handle add_to_keyrepeat

        break;
      }
    }
  }
}
