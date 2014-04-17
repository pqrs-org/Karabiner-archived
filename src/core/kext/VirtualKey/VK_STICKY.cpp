#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "KeyCodeModifierFlagPairs.hpp"
#include "VK_STICKY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_STICKY::handle(const Params_KeyboardEventCallBack& params)
  {
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

    if (params.key == KeyCode::VK_STICKY_ALL_FORCE_OFF) {
      if (isFirstKeyDownEvent) {
        FlagStatus::globalFlagStatus().sticky_clear();
      }
      goto remapped;
    }

    return false;

  remapped:
    EventOutputQueue::FireModifiers::fire();
    return true;
  }
}
