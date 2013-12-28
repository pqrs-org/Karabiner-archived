#include <IOKit/IOLib.h>

#include "ConsumerToKey.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ConsumerToKey::ConsumerToKey(void) : index_(0)
    {}

    ConsumerToKey::~ConsumerToKey(void)
    {}

    void
    ConsumerToKey::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        {
          switch (index_) {
            case 0:
              fromKey_.key = ConsumerKeyCode(newval);
              consumertoconsumer_.add(ConsumerKeyCode(newval));
              consumertoconsumer_.add(ConsumerKeyCode::VK_NONE);
              break;
            default:
              IOLOG_ERROR("Invalid ConsumerToKey::add\n");
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid ConsumerToKey::add\n");
              break;

            case 1:
              // pass-through (== no break)
              keytokey_.add(KeyCode::VK_PSEUDO_KEY);
              keytokey_.add(fromKey_.flags);
            default:
              keytokey_.add(KeyCode(newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              break;
            case 1:
              fromKey_.flags = Flags(newval);
              break;
            default:
              keytokey_.add(Flags(newval));
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          if (Option::NOREPEAT == newval ||
              Option::KEYTOKEY_BEFORE_KEYDOWN == newval ||
              Option::KEYTOKEY_AFTER_KEYUP == newval) {
            keytokey_.add(Option(newval));
          } else {
            IOLOG_ERROR("ConsumerToKey::add unknown option:%d\n", newval);
          }
          break;
        }

        case BRIDGE_DATATYPE_DELAYUNTILREPEAT:
        case BRIDGE_DATATYPE_KEYREPEAT:
        {
          keytokey_.add(datatype, newval);
          break;
        }

        default:
          IOLOG_ERROR("ConsumerToKey::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    ConsumerToKey::remap(RemapConsumerParams& remapParams)
    {
      if (! fromkeychecker_.isFromKey(remapParams.params.ex_iskeydown, remapParams.params.key, FlagStatus::makeFlags(), fromKey_.key, fromKey_.flags)) return false;

      bool result = consumertoconsumer_.remap(remapParams);
      if (! result) return false;

      return keytokey_.call_remap_with_VK_PSEUDO_KEY(remapParams.params.eventType);
    }
  }
}
