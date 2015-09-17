#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "KeyCodeModifierFlagPairs.hpp"
#include "VK_STICKY.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_STICKY::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  bool isFirstKeyDownEvent = (params.ex_iskeydown && params.repeat == false);

  {
    ModifierFlag modifierFlag =
        KeyCodeModifierFlagPairs::getModifierFlag(params.key, KeyCodeModifierFlagPairs::KeyCodeType::VK_STICKY);
    if (modifierFlag != ModifierFlag::ZERO) {
      if (isFirstKeyDownEvent) {
        FlagStatus::globalFlagStatus().sticky_toggle(modifierFlag);
      }
      goto remapped;
    }
  }
  {
    ModifierFlag modifierFlag =
        KeyCodeModifierFlagPairs::getModifierFlag(params.key, KeyCodeModifierFlagPairs::KeyCodeType::VK_STICKY_FORCE_ON);
    if (modifierFlag != ModifierFlag::ZERO) {
      if (isFirstKeyDownEvent) {
        FlagStatus::globalFlagStatus().sticky_increase(modifierFlag);
      }
      goto remapped;
    }
  }
  {
    ModifierFlag modifierFlag =
        KeyCodeModifierFlagPairs::getModifierFlag(params.key, KeyCodeModifierFlagPairs::KeyCodeType::VK_STICKY_FORCE_OFF);
    if (modifierFlag != ModifierFlag::ZERO) {
      if (isFirstKeyDownEvent) {
        FlagStatus::globalFlagStatus().sticky_decrease(modifierFlag);
      }
      goto remapped;
    }
  }

#define HANDLE_KEYCODE(KEYCODE, METHOD)          \
  {                                              \
    if (params.key == KEYCODE) {                 \
      if (isFirstKeyDownEvent) {                 \
        FlagStatus::globalFlagStatus().METHOD(); \
      }                                          \
      goto remapped;                             \
    }                                            \
  }

  HANDLE_KEYCODE(KeyCode::VK_STICKY_ACTIVE_MODIFIERS_TOGGLE, sticky_active_modifiers_toggle);
  HANDLE_KEYCODE(KeyCode::VK_STICKY_ACTIVE_MODIFIERS_FORCE_ON, sticky_active_modifiers_increase);
  HANDLE_KEYCODE(KeyCode::VK_STICKY_ACTIVE_MODIFIERS_FORCE_OFF, sticky_active_modifiers_decrease);
  HANDLE_KEYCODE(KeyCode::VK_STICKY_ALL_FORCE_OFF, sticky_clear);

  return false;

remapped:
  EventOutputQueue::FireModifiers::fire(autogenId, physicalEventType);
  return true;
}
}
