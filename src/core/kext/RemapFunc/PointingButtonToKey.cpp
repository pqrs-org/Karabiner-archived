#include <IOKit/IOLib.h>

#include "IOLogWrapper.hpp"
#include "PointingButtonToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    PointingButtonToKey::PointingButtonToKey(void) : index_(0)
    {}

    PointingButtonToKey::~PointingButtonToKey(void)
    {
      disabled_callback();
    }

    void
    PointingButtonToKey::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          switch (index_) {
            case 0:
              fromButton_.button = PointingButton(newval);
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
              fromButton_.flags = Flags(newval);
              break;
            default:
              keytokey_.add(Flags(newval));
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          if (Option::NOREPEAT == Option(newval) ||
              Option::KEYTOKEY_BEFORE_KEYDOWN == Option(newval) ||
              Option::KEYTOKEY_AFTER_KEYUP == Option(newval)) {
            keytokey_.add(Option(newval));
          } else {
            IOLOG_ERROR("PointingButtonToKey::add unknown option:%d\n", newval);
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
          IOLOG_ERROR("PointingButtonToKey::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    PointingButtonToKey::remap(RemapParams& remapParams)
    {
      Params_RelativePointerEventCallback* params = remapParams.paramsUnion.get_Params_RelativePointerEventCallback();
      if (! params) return false;

      if (remapParams.isremapped) return false;
      if (! fromkeychecker_.isFromPointingButton(*params, fromButton_.button, fromButton_.flags)) return false;
      remapParams.isremapped = true;

      if (params->ex_isbuttondown) {
        retractInput();
      } else {
        restoreInput();
      }

      return keytokey_.call_remap_with_VK_PSEUDO_KEY(params->ex_isbuttondown ? EventType::DOWN : EventType::UP);
    }

    void
    PointingButtonToKey::disabled_callback(void)
    {
      if (fromkeychecker_.isactive()) {
        fromkeychecker_.deactivate();
        restoreInput();
      }
    }

    void
    PointingButtonToKey::retractInput(void)
    {
      ButtonStatus::decrease(fromButton_.button);
    }

    void
    PointingButtonToKey::restoreInput(void)
    {
      ButtonStatus::increase(fromButton_.button);
    }
  }
}
