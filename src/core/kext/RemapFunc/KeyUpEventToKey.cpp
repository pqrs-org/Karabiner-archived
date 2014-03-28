#include "EventWatcher.hpp"
#include "KeyUpEventToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    KeyUpEventToKey::KeyUpEventToKey(void)
    {
      keytokey_.add(KeyCode::VK_PSEUDO_KEY);
    }

    void
    KeyUpEventToKey::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          if (fromEvent_.getType() == FromEvent::Type::NONE) {
            fromEvent_ = FromEvent(datatype, newval);
          } else {
            keytokey_.add(datatype, newval);
          }
          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          if (keytokey_.toKeysSize() == 0) {
            fromFlags_ = Flags(newval);
          } else {
            keytokey_.add(datatype, newval);
          }
          break;
        }

        default:
          keytokey_.add(datatype, newval);
          break;
      }
    }

    bool
    KeyUpEventToKey::remap(RemapParams& remapParams)
    {
      if (remapParams.isremapped) return false;

      if (fromEvent_.isTargetDownEvent(remapParams.paramsUnion)) {
        remapParams.isremapped = true;
        EventWatcher::undo();
        return true;
      }

      if (fromEvent_.isTargetUpEvent(remapParams.paramsUnion) &&
          FlagStatus::makeFlags().isOn(fromFlags_)) {
        remapParams.isremapped = true;
        EventWatcher::on();

        FlagStatus::decrease(fromFlags_);
        keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
        keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);
        FlagStatus::increase(fromFlags_);

        return true;
      }

      return false;
    }
  }
}
