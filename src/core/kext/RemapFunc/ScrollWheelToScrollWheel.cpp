#include "ScrollWheelToScrollWheel.hpp"
#include "EventOutputQueue.hpp"
#include "PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ScrollWheelToScrollWheel::ScrollWheelToScrollWheel(void) : index_(0)
    {}

    ScrollWheelToScrollWheel::~ScrollWheelToScrollWheel(void)
    {}

    void
    ScrollWheelToScrollWheel::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              fromFlags_ = newval;
              break;
            default:
              toFlags_ = newval;
              break;
          }
          ++index_;

          break;
        }

        default:
          IOLOG_ERROR("ScrollWheelToScrollWheel::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    ScrollWheelToScrollWheel::remap(RemapPointingParams_scroll& remapParams)
    {
      if (remapParams.isremapped) return false;
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) return false;

      remapParams.isremapped = true;

      FlagStatus::temporary_decrease(fromFlags_);
      FlagStatus::temporary_increase(toFlags_);

      EventOutputQueue::FireScrollWheel::fire(remapParams.params);
      RemapFunc::PointingRelativeToScroll::cancelScroll();

      return true;
    }
  }
}
