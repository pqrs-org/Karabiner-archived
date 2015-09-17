#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "PointingRelativeToScroll.hpp"
#include "ScrollWheelToScrollWheel.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void ScrollWheelToScrollWheel::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_MODIFIERFLAG: {
    switch (index_) {
    case 0:
      fromModifierFlags_.push_back(ModifierFlag(datatype, newval));
      break;
    default:
      toModifierFlags_.push_back(ModifierFlag(datatype, newval));
      break;
    }
    break;
  }

  case BRIDGE_DATATYPE_MODIFIERFLAGS_END:
    ++index_;
    break;

  default:
    IOLOG_ERROR("ScrollWheelToScrollWheel::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

bool ScrollWheelToScrollWheel::remap(RemapParams& remapParams) {
  auto params = remapParams.paramsBase.get_Params_ScrollWheelEventCallback();
  if (!params) return false;

  if (remapParams.isremapped) return false;
  if (!FlagStatus::globalFlagStatus().isOn(fromModifierFlags_)) return false;

  remapParams.isremapped = true;

  FlagStatus::globalFlagStatus().temporary_decrease(fromModifierFlags_);
  FlagStatus::globalFlagStatus().temporary_increase(toModifierFlags_);

  EventOutputQueue::FireScrollWheel::fire(*params, autogenId_, remapParams.physicalEventType);
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
  FlagStatus::globalFlagStatus().temporary_decrease(toModifierFlags_);
  FlagStatus::globalFlagStatus().temporary_increase(fromModifierFlags_);

  return true;
}
}
}
