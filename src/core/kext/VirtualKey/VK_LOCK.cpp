#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "KeyCodeModifierFlagPairs.hpp"
#include "VK_LOCK.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_LOCK::handle(const Params_KeyboardEventCallBack& params)
  {
    bool isFirstKeyDownEvent = (params.ex_iskeydown && params.repeat == false);

    {
      ModifierFlag modifierFlag =
        KeyCodeModifierFlagPairs::getModifierFlag(params.key, KeyCodeModifierFlagPairs::KeyCodeType::VK_LOCK);
      if (modifierFlag != ModifierFlag::ZERO) {
        if (isFirstKeyDownEvent) {
          FlagStatus::globalFlagStatus().lock_toggle(modifierFlag);
        }
        goto remapped;
      }
    }
    {
      ModifierFlag modifierFlag =
        KeyCodeModifierFlagPairs::getModifierFlag(params.key, KeyCodeModifierFlagPairs::KeyCodeType::VK_LOCK_FORCE_ON);
      if (modifierFlag != ModifierFlag::ZERO) {
        if (isFirstKeyDownEvent) {
          FlagStatus::globalFlagStatus().lock_increase(modifierFlag);
        }
        goto remapped;
      }
    }
    {
      ModifierFlag modifierFlag =
        KeyCodeModifierFlagPairs::getModifierFlag(params.key, KeyCodeModifierFlagPairs::KeyCodeType::VK_LOCK_FORCE_OFF);
      if (modifierFlag != ModifierFlag::ZERO) {
        if (isFirstKeyDownEvent) {
          FlagStatus::globalFlagStatus().lock_decrease(modifierFlag);
        }
        goto remapped;
      }
    }

    if (params.key == KeyCode::VK_LOCK_ALL_FORCE_OFF) {
      if (isFirstKeyDownEvent) {
        FlagStatus::globalFlagStatus().lock_clear();
      }
      goto remapped;
    }

    return false;

  remapped:
    EventOutputQueue::FireModifiers::fire();
    return true;
  }
}
