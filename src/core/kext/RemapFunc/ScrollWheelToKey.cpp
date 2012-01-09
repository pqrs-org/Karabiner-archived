#include "Config.hpp"
#include "ScrollWheelToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ScrollWheelToKey::ScrollWheelToKey(void) : index_(0), isLastEventRemapped_(false), firstEventOfContinuousScrollWheelEvents_(ScrollWheel::NONE)
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

      // We treat all continuous scrollwheel events are same as the first event of continuous scrollwheel events.
      //
      // Because Trackpad and Magic Mouse sends following events when we move finger diagonally.
      //   deltaAxis(-1,1,0), fixedDelta(-20762,23778,0), pointDelta(-3,3,0)
      //   deltaAxis(-1,1,0), fixedDelta(-19385,23991,0), pointDelta(-2,3,0)
      //   deltaAxis(0,1,0),  fixedDelta(0,44986,0),      pointDelta(-3,3,0)
      //   ...
      //
      // There are some non-diagonal events.
      // But we want to treat them as diagonal events.
      // Therefore, we remember the first event and replace following events by it.

      if (continuousScrollEvent_ic_.getmillisec() < CONTINUOUS_SCROLLWHEEL_EVENT_THRESHOLD &&
          firstEventOfContinuousScrollWheelEvents_ != ScrollWheel::NONE) {
        // continuous scrollwheel event.
        scrollwheel = firstEventOfContinuousScrollWheelEvents_;

      } else {
        scrollwheel = ScrollWheel::getScrollWheelFromDelta(remapParams.params.fixedDelta1,
                                                           remapParams.params.fixedDelta2);
        firstEventOfContinuousScrollWheelEvents_ = scrollwheel;
      }
      continuousScrollEvent_ic_.begin();

      // ------------------------------------------------------------
      if (remapParams.isremapped) goto notchange;
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) goto notchange;

      if (scrollwheel != fromScrollWheel_) goto notchange;

      remapParams.isremapped = true;

      if (! isLastEventRemapped_ ||
          keyrepeat_ic_.getmillisec() > static_cast<uint32_t>(Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_scrollwheeltokey_keyrepeat_wait))) {
        keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
        keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);

        keyrepeat_ic_.begin();
      }

      isLastEventRemapped_ = true;

      return true;

    notchange:
      isLastEventRemapped_ = false;
      return false;
    }
  }
}
