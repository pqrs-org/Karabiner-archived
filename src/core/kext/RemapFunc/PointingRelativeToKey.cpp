#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "Config.hpp"
#include "EventWatcher.hpp"
#include "IOLogWrapper.hpp"
#include "PointingRelativeToKey.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void PointingRelativeToKey::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_POINTINGRELATIVE: {
    PointingRelative val(newval);
    size_t keytokey_index = KEYTOKEY__END__;
    if (val == PointingRelative::UP) {
      keytokey_index = KEYTOKEY_UP;
    }
    if (val == PointingRelative::DOWN) {
      keytokey_index = KEYTOKEY_DOWN;
    }
    if (val == PointingRelative::LEFT) {
      keytokey_index = KEYTOKEY_LEFT;
    }
    if (val == PointingRelative::RIGHT) {
      keytokey_index = KEYTOKEY_RIGHT;
    }
    if (val == PointingRelative::ANY) {
      keytokey_index = KEYTOKEY_ANY;
    }

    if (keytokey_index != KEYTOKEY__END__) {
      current_keytokey_ = &(keytokey_[keytokey_index]);
      defined_[keytokey_index] = true;
    }
    break;
  }

  case BRIDGE_DATATYPE_KEYCODE:
  case BRIDGE_DATATYPE_CONSUMERKEYCODE:
  case BRIDGE_DATATYPE_POINTINGBUTTON:
  case BRIDGE_DATATYPE_OPTION: {
    if (!current_keytokey_) {
      IOLOG_ERROR("Invalid PointingRelativeToKey::add\n");
    } else {
      current_keytokey_->add(datatype, newval);
    }

    break;
  }

  case BRIDGE_DATATYPE_MODIFIERFLAG:
  case BRIDGE_DATATYPE_MODIFIERFLAGS_END: {
    if (!current_keytokey_) {
      fromModifierFlags_.push_back(ModifierFlag(datatype, newval));

      for (size_t i = 0; i < KEYTOKEY__END__; ++i) {
        keytokey_[i].add(datatype, newval);
      }
    } else {
      current_keytokey_->add(datatype, newval);
    }
    break;
  }

  default:
    IOLOG_ERROR("PointingRelativeToKey::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

bool PointingRelativeToKey::remap(RemapParams& remapParams) {
  auto params = remapParams.paramsBase.get_Params_RelativePointerEventCallback();
  if (!params) return false;

  // ------------------------------------------------------------
  if (remapParams.isremapped) return false;
  if (!FlagStatus::globalFlagStatus().isOn(fromModifierFlags_)) return false;
  // skip button press/release events.
  if (params->ex_button != PointingButton::NONE) return false;

  remapParams.isremapped = true;

  // ------------------------------------------------------------
  uint32_t detection_interval = static_cast<uint32_t>(Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_pointingrelativetokey_detection_interval));
  if (keyrepeat_ic_.getmillisec() > detection_interval) {
    deltaBuffer_dx_.push(params->dx);
    deltaBuffer_dy_.push(params->dy);
  }

  if (deltaBuffer_dx_.isFull() || deltaBuffer_dy_.isFull()) {
    int dx = deltaBuffer_dx_.sum();
    int dy = deltaBuffer_dy_.sum();

    size_t keytokey_index = KEYTOKEY_ANY;
    PointingRelative pr = PointingRelative::getPointingRelativeFromDelta(dx, dy);
    if (pr == PointingRelative::UP) {
      keytokey_index = KEYTOKEY_UP;
    }
    if (pr == PointingRelative::DOWN) {
      keytokey_index = KEYTOKEY_DOWN;
    }
    if (pr == PointingRelative::LEFT) {
      keytokey_index = KEYTOKEY_LEFT;
    }
    if (pr == PointingRelative::RIGHT) {
      keytokey_index = KEYTOKEY_RIGHT;
    }

    if (!defined_[keytokey_index]) {
      // fallback
      keytokey_index = KEYTOKEY_ANY;
    }

    // ----------------------------------------
    keytokey_[keytokey_index].call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, remapParams.physicalEventType);
    keytokey_[keytokey_index].call_remap_with_VK_PSEUDO_KEY(EventType::UP, remapParams.physicalEventType);

    // clear temporary flags.
    FlagStatus::globalFlagStatus().set();

    // We need to call EventWatcher::on here.
    // See the comments in EventInputQueue::fire_timer_callback.
    EventWatcher::on();

    deltaBuffer_dx_.clear();
    deltaBuffer_dy_.clear();
    keyrepeat_ic_.begin();
  }

  return true;
}
}
}
