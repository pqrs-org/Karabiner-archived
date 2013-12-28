#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "PointingRelativeToScroll.hpp"
#include "ScrollWheelToScrollWheel.hpp"

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
              fromFlags_ = Flags(newval);
              break;
            default:
              toFlags_ = Flags(newval);
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

      // We need to restore temporary flags.
      // Because normal cursor move event don't restore temporary_count_.
      // (See EventInputQueue::push_RelativePointerEventCallback.)
      //
      // ------------------------------------------------------------
      // For example:
      // This autogen changes option+scroll to scroll. (strip option modifier.)
      //   <autogen>__ScrollWheelToScrollWheel__ ModifierFlag::OPTION_L, ModifierFlag::NONE</autogen>
      //
      // Considering the following operation with this autogen, we need to restore temporary flags at (3).
      // (1) option+left click
      // (2) drag mouse (option+left drag)
      // (3) scroll (strip option)
      // (4) drag mouse (option+left drag)
      // ------------------------------------------------------------
      FlagStatus::temporary_decrease(toFlags_);
      FlagStatus::temporary_increase(fromFlags_);

      return true;
    }
  }
}
