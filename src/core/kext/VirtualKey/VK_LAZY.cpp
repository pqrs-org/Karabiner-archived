#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "IOLogWrapper.hpp"
#include "Params.hpp"
#include "VK_LAZY.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_LAZY::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  ModifierFlag f = getModifierFlag(params.key);
  if (f == ModifierFlag::ZERO) return false;

  // ----------------------------------------
  if (params.repeat) return true;

  if (params.eventType == EventType::DOWN) {
    FlagStatus::globalFlagStatus().lazy_increase(f);
  } else if (params.eventType == EventType::UP) {
    FlagStatus::globalFlagStatus().lazy_decrease(f);
  } else {
    IOLOG_ERROR("Handle_VK_LAZY invalid EventType.\n");
  }

  return true;
}

ModifierFlag
VirtualKey::VK_LAZY::getModifierFlag(KeyCode keycode) {
  /**/ if (keycode == KeyCode::VK_LAZY_COMMAND_L) {
    return ModifierFlag::COMMAND_L;
  } else if (keycode == KeyCode::VK_LAZY_COMMAND_R) {
    return ModifierFlag::COMMAND_R;
  } else if (keycode == KeyCode::VK_LAZY_CONTROL_L) {
    return ModifierFlag::CONTROL_L;
  } else if (keycode == KeyCode::VK_LAZY_CONTROL_R) {
    return ModifierFlag::CONTROL_R;
  } else if (keycode == KeyCode::VK_LAZY_FN) {
    return ModifierFlag::FN;
  } else if (keycode == KeyCode::VK_LAZY_OPTION_L) {
    return ModifierFlag::OPTION_L;
  } else if (keycode == KeyCode::VK_LAZY_OPTION_R) {
    return ModifierFlag::OPTION_R;
  } else if (keycode == KeyCode::VK_LAZY_SHIFT_L) {
    return ModifierFlag::SHIFT_L;
  } else if (keycode == KeyCode::VK_LAZY_SHIFT_R) {
    return ModifierFlag::SHIFT_R;
  }

  return ModifierFlag::ZERO;
}
}
