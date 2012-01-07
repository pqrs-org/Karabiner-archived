#include "ScrollWheelToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ScrollWheelToKey::ScrollWheelToKey(void) : index_(0)
    {}

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
      if (remapParams.isremapped) return false;
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) return false;

      ScrollWheel scrollwheel = ScrollWheel::getScrollWheelFromDelta(remapParams.params.fixedDelta1,
                                                                     remapParams.params.fixedDelta2);
      if (scrollwheel != fromScrollWheel_) return false;

      remapParams.isremapped = true;

      keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
      keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);

      return true;
    }
  }
}
