#include "Config.hpp"
#include "ScrollWheelToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ScrollWheelToKey::ScrollWheelToKey(void) : index_(0), isLastEventRemapped_(false)
    {
      ic_.begin();
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
              fromScrollWheel_ = newval;
              break;
            default:
              IOLOG_ERROR("Invalid ScrollWheelToKey::add\n");
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_KEYCODE:
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
              IOLOG_ERROR("Invalid ScrollWheelToKey::add\n");
              break;
            case 1:
              fromFlags_ = newval;
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
    ScrollWheelToKey::remap(RemapPointingParams_scroll& remapParams)
    {
      ScrollWheel scrollwheel = ScrollWheel::NONE;

      if (remapParams.isremapped) goto notchange;
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) goto notchange;

      scrollwheel = ScrollWheel::getScrollWheelFromDelta(remapParams.params.fixedDelta1,
                                                         remapParams.params.fixedDelta2);
      if (scrollwheel == ScrollWheel::NONE) {
        // Devices which has momentum scroll (Trackpad, Magic Mouse) sends null scroll event such as
        // "deltaAxis(0,0,0), fixedDelta(0,0,0), pointDelta(0,0,0)".
        // We need to ignore this null scroll events if it is related to remapped scroll event.
        if (isLastEventRemapped_) {
          remapParams.isremapped = true;
          return true;

        } else {
          goto notchange;
        }
      }

      if (scrollwheel != fromScrollWheel_) goto notchange;

      remapParams.isremapped = true;

      if (! isLastEventRemapped_ ||
          ic_.getmillisec() > static_cast<uint32_t>(Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_scrollwheeltokey_keyrepeat_wait))) {
        keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
        keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);

        ic_.begin();
      }
      isLastEventRemapped_ = true;

      return true;

    notchange:
      isLastEventRemapped_ = false;
      return false;
    }
  }
}
