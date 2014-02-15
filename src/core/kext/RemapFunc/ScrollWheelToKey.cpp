#include <IOKit/IOLib.h>

#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "ScrollWheelToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ScrollWheelToKey::ScrollWheelToKey(void) :
      index_(0),
      firstScrollWheelEvent_(ScrollWheel::NONE),
      isContinuousScrollEventRemapped_(false)
    {
      continuousScrollEvent_ic_.begin();
      keyrepeat_ic_.begin();
    }

    ScrollWheelToKey::~ScrollWheelToKey(void)
    {}

    void
    ScrollWheelToKey::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_SCROLLWHEEL:
        {
          switch (index_) {
            case 0:
              fromScrollWheel_ = ScrollWheel(newval);
              break;
            default:
              IOLOG_ERROR("Invalid ScrollWheelToKey::add\n");
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_KEYCODE:
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid ScrollWheelToKey::add\n");
              break;

            case 1:
              // pass-through (== no break)
              keytokey_.add(KeyCode::VK_PSEUDO_KEY);
              keytokey_.add(fromFlags_);
            default:
              keytokey_.add(datatype, newval);
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid ScrollWheelToKey::add\n");
              break;
            case 1:
              fromFlags_ = Flags(newval);
              break;
            default:
              keytokey_.add(Flags(newval));
              break;
          }
          break;
        }

        default:
          IOLOG_ERROR("ScrollWheelToKey::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    ScrollWheelToKey::remap(RemapParams& remapParams)
    {
      Params_ScrollWheelEventCallback* params = remapParams.paramsUnion.get_Params_ScrollWheelEventCallback();
      if (! params) return false;

      // ------------------------------------------------------------
      // We treat the first event of continuous events.
      // Ignore following events if the first event is remapped, otherwise pass through them.
      int millisec = continuousScrollEvent_ic_.getmillisec();
      continuousScrollEvent_ic_.begin();

      if (millisec < CONTINUOUS_SCROLLWHEEL_EVENT_THRESHOLD && firstScrollWheelEvent_ != ScrollWheel::NONE) {
        if (isContinuousScrollEventRemapped_) {
          remapParams.isremapped = true;
          return true;
        } else {
          return false;
        }
      }

      // ------------------------------------------------------------
      // Handling the first event of continuous events.
      isContinuousScrollEventRemapped_ = false;

      // --------------------
      // Ignore first null event.
      firstScrollWheelEvent_ = ScrollWheel::getScrollWheelFromDelta(params->fixedDelta1,
                                                                    params->fixedDelta2);
      if (firstScrollWheelEvent_ == ScrollWheel::NONE) {
        return false;
      }

      // --------------------
      if (remapParams.isremapped) return false;
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) return false;

      if (fromScrollWheel_ != firstScrollWheelEvent_) return false;

      remapParams.isremapped = true;
      isContinuousScrollEventRemapped_ = true;

      if (keyrepeat_ic_.getmillisec() > static_cast<uint32_t>(Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_scrollwheeltokey_keyrepeat_wait))) {
        keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
        keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);

        keyrepeat_ic_.begin();
      }

      return true;
    }
  }
}
