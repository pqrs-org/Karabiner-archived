#include "PointingButtonToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    PointingButtonToKey::initialize(void)
    {
      keytokey_.initialize();
    }

    void
    PointingButtonToKey::terminate(void)
    {
      keytokey_.terminate();
    }

    void
    PointingButtonToKey::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          switch (index_) {
            case 0:
              fromButton_.button = newval;
              break;
            default:
              IOLOG_ERROR("Invalid PointingButtonToKey::add\n");
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid PointingButtonToKey::add\n");
              break;

            case 1:
              // pass-through (== no break)
              keytokey_.add(KeyCode::VK_PSEUDO_KEY);
              keytokey_.add(fromButton_.flags);
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
              IOLOG_ERROR("Invalid PointingButtonToKey::add\n");
              break;
            case 1:
              fromButton_.flags = newval;
              break;
            default:
              keytokey_.add(Flags(newval));
              break;
          }
          break;
        }

        default:
          IOLOG_ERROR("PointingButtonToKey::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    PointingButtonToKey::remap(RemapPointingParams_relative& remapParams)
    {
      if (remapParams.isremapped) return false;
      if (! fromkeychecker_.isFromPointingButton(remapParams.params, fromButton_.button, fromButton_.flags)) return false;
      remapParams.isremapped = true;

      if (remapParams.params.ex_isbuttondown) {
        ButtonStatus::decrease(fromButton_.button);
      } else {
        ButtonStatus::increase(fromButton_.button);
      }

      return keytokey_.call_remap_with_VK_PSEUDO_KEY(remapParams.params.ex_isbuttondown ? EventType::DOWN : EventType::UP);
    }
  }
}
