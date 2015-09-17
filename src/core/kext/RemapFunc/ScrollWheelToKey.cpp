#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "ScrollWheelToKey.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void ScrollWheelToKey::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_SCROLLWHEEL: {
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
  case BRIDGE_DATATYPE_POINTINGBUTTON: {
    switch (index_) {
    case 0:
      IOLOG_ERROR("Invalid ScrollWheelToKey::add\n");
      break;

    default:
      keytokey_.add(datatype, newval);
      break;
    }
    ++index_;

    break;
  }

  case BRIDGE_DATATYPE_MODIFIERFLAG:
  case BRIDGE_DATATYPE_MODIFIERFLAGS_END: {
    switch (index_) {
    case 0:
      IOLOG_ERROR("Invalid ScrollWheelToKey::add\n");
      break;

    default:
      if (index_ == 1) {
        fromModifierFlags_.push_back(ModifierFlag(datatype, newval));
      }
      keytokey_.add(datatype, newval);
      break;
    }
    break;
  }

  default:
    IOLOG_ERROR("ScrollWheelToKey::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

bool ScrollWheelToKey::remap(RemapParams& remapParams) {
  auto params = remapParams.paramsBase.get_Params_ScrollWheelEventCallback();
  if (!params) return false;

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
  if (!FlagStatus::globalFlagStatus().isOn(fromModifierFlags_)) return false;

  if (fromScrollWheel_ != firstScrollWheelEvent_) return false;

  remapParams.isremapped = true;
  isContinuousScrollEventRemapped_ = true;

  if (keyrepeat_ic_.getmillisec() > static_cast<uint32_t>(Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_scrollwheeltokey_keyrepeat_wait))) {
    keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, remapParams.physicalEventType);
    keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::UP, remapParams.physicalEventType);

    keyrepeat_ic_.begin();
  }

  return true;
}
}
}
