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
}
