#include <IOKit/IOLib.h>

#include "IOLogWrapper.hpp"
#include "KeyToConsumer.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    KeyToConsumer::KeyToConsumer(void) : index_(0)
    {}

    KeyToConsumer::~KeyToConsumer(void)
    {}

    void
    KeyToConsumer::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              fromKey_.key = KeyCode(newval);
              keytokey_.add(KeyCode(newval));
              keytokey_.add(KeyCode::VK_NONE);
              break;
            default:
              IOLOG_ERROR("Invalid KeyToConsumer::add\n");
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid KeyToConsumer::add\n");
              break;

            case 1:
              // pass-through (== no break)
              consumertoconsumer_.add(ConsumerKeyCode::VK_PSEUDO_KEY);
              consumertoconsumer_.add(fromKey_.flags);
            default:
              consumertoconsumer_.add(ConsumerKeyCode(newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid KeyToConsumer::add\n");
              break;
            case 1:
              fromKey_.flags = Flags(newval);
              break;
            default:
              consumertoconsumer_.add(Flags(newval));
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          if (Option::NOREPEAT == Option(newval)) {
            consumertoconsumer_.add(Option(newval));
          } else {
            IOLOG_ERROR("KeyToConsumer::add unknown option:%d\n", newval);
          }
          break;
        }

        default:
          IOLOG_ERROR("KeyToConsumer::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    KeyToConsumer::remap(RemapParams& remapParams)
    {
      Params_KeyboardEventCallBack* params = remapParams.paramsUnion.get_Params_KeyboardEventCallBack();
      if (! params) return false;

      if (! fromkeychecker_.isFromKey(params->ex_iskeydown, params->key, FlagStatus::makeFlags(), fromKey_.key, fromKey_.flags)) return false;

      bool result = keytokey_.remap(remapParams);
      if (! result) return false;

      // ----------------------------------------
      return consumertoconsumer_.call_remap_with_VK_PSEUDO_KEY(params->ex_iskeydown ? EventType::DOWN : EventType::UP);
    }
  }
}
