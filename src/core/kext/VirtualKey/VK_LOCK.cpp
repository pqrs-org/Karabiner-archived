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

#define MODIFY_FLAGSTATUS(KEYCODETYPE, METHOD) {                                                    \
    ModifierFlag modifierFlag = KeyCodeModifierFlagPairs::getModifierFlag(params.key, KEYCODETYPE); \
    if (modifierFlag != ModifierFlag::ZERO) {                                                       \
      if (isFirstKeyDownEvent) {                                                                    \
        FlagStatus::globalFlagStatus().METHOD(modifierFlag);                                        \
      }                                                                                             \
      goto remapped;                                                                                \
    }                                                                                               \
}

    // VK_LOCK
    MODIFY_FLAGSTATUS(KeyCodeModifierFlagPairs::KeyCodeType::VK_LOCK,           lock_toggle);
    MODIFY_FLAGSTATUS(KeyCodeModifierFlagPairs::KeyCodeType::VK_LOCK_FORCE_ON,  lock_increase);
    MODIFY_FLAGSTATUS(KeyCodeModifierFlagPairs::KeyCodeType::VK_LOCK_FORCE_OFF, lock_decrease);

    // VK_NEGATIVE_LOCK
    MODIFY_FLAGSTATUS(KeyCodeModifierFlagPairs::KeyCodeType::VK_NEGATIVE_LOCK,           negative_lock_toggle);
    MODIFY_FLAGSTATUS(KeyCodeModifierFlagPairs::KeyCodeType::VK_NEGATIVE_LOCK_FORCE_ON,  negative_lock_increase);
    MODIFY_FLAGSTATUS(KeyCodeModifierFlagPairs::KeyCodeType::VK_NEGATIVE_LOCK_FORCE_OFF, negative_lock_decrease);

    if (params.key == KeyCode::VK_LOCK_ALL_FORCE_OFF) {
      if (isFirstKeyDownEvent) {
        FlagStatus::globalFlagStatus().lock_clear();
      }
      goto remapped;
    }

    if (params.key == KeyCode::VK_NEGATIVE_LOCK_ALL_FORCE_OFF) {
      if (isFirstKeyDownEvent) {
        FlagStatus::globalFlagStatus().negative_lock_clear();
      }
      goto remapped;
    }

    return false;

  remapped:
    EventOutputQueue::FireModifiers::fire();
    return true;
  }
}
