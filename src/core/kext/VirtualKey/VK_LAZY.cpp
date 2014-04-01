#include <IOKit/IOLib.h>

#include "CallbackWrapper.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "IOLogWrapper.hpp"
#include "VK_LAZY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_LAZY::handle(const Params_KeyboardEventCallBack& params)
  {
    ModifierFlag f = getModifierFlag(params.key);
    if (f == ModifierFlag::NONE) return false;

    // ----------------------------------------
    if (params.repeat) return true;

    if (params.eventType == EventType::DOWN) {
      FlagStatus::globalFlagStatus().increase(f);
    } else if (params.eventType == EventType::UP) {
      FlagStatus::globalFlagStatus().decrease(f);
    } else {
      IOLOG_ERROR("Handle_VK_LAZY invalid EventType.\n");
    }

    return true;
  }

  ModifierFlag
  VirtualKey::VK_LAZY::getModifierFlag(KeyCode keycode)
  {
    /**/ if (keycode == KeyCode::VK_LAZY_COMMAND_L) { return ModifierFlag::COMMAND_L; }
    else if (keycode == KeyCode::VK_LAZY_COMMAND_R) { return ModifierFlag::COMMAND_R; }
    else if (keycode == KeyCode::VK_LAZY_CONTROL_L) { return ModifierFlag::CONTROL_L; }
    else if (keycode == KeyCode::VK_LAZY_CONTROL_R) { return ModifierFlag::CONTROL_R; }
    else if (keycode == KeyCode::VK_LAZY_FN)        { return ModifierFlag::FN;        }
    else if (keycode == KeyCode::VK_LAZY_OPTION_L)  { return ModifierFlag::OPTION_L;  }
    else if (keycode == KeyCode::VK_LAZY_OPTION_R)  { return ModifierFlag::OPTION_R;  }
    else if (keycode == KeyCode::VK_LAZY_SHIFT_L)   { return ModifierFlag::SHIFT_L;   }
    else if (keycode == KeyCode::VK_LAZY_SHIFT_R)   { return ModifierFlag::SHIFT_R;   }

    return ModifierFlag::NONE;
  }
}
