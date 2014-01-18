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
        // XXX
        break;

      case Type::POINTING_BUTTON:
        // XXX
        break;
    }
  }
}
