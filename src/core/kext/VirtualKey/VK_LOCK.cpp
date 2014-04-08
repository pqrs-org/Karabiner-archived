#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_LOCK.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_LOCK::handle(const Params_KeyboardEventCallBack& params)
  {
    ModifierFlag modifierFlag;
    bool force_on = false;
    bool force_off = false;

    /**/ if (params.key == KeyCode::VK_LOCK_COMMAND_L) { modifierFlag = ModifierFlag::COMMAND_L; }
    else if (params.key == KeyCode::VK_LOCK_COMMAND_R) { modifierFlag = ModifierFlag::COMMAND_R; }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_L) { modifierFlag = ModifierFlag::CONTROL_L; }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_R) { modifierFlag = ModifierFlag::CONTROL_R; }
    else if (params.key == KeyCode::VK_LOCK_FN)        { modifierFlag = ModifierFlag::FN;        }
    else if (params.key == KeyCode::VK_LOCK_OPTION_L)  { modifierFlag = ModifierFlag::OPTION_L;  }
    else if (params.key == KeyCode::VK_LOCK_OPTION_R)  { modifierFlag = ModifierFlag::OPTION_R;  }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_L)   { modifierFlag = ModifierFlag::SHIFT_L;   }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_R)   { modifierFlag = ModifierFlag::SHIFT_R;   }
    else if (params.key == KeyCode::VK_LOCK_EXTRA1)    { modifierFlag = ModifierFlag::EXTRA1;    }
    else if (params.key == KeyCode::VK_LOCK_EXTRA2)    { modifierFlag = ModifierFlag::EXTRA2;    }
    else if (params.key == KeyCode::VK_LOCK_EXTRA3)    { modifierFlag = ModifierFlag::EXTRA3;    }
    else if (params.key == KeyCode::VK_LOCK_EXTRA4)    { modifierFlag = ModifierFlag::EXTRA4;    }
    else if (params.key == KeyCode::VK_LOCK_EXTRA5)    { modifierFlag = ModifierFlag::EXTRA5;    }

    else if (params.key == KeyCode::VK_LOCK_COMMAND_L_FORCE_ON) { modifierFlag = ModifierFlag::COMMAND_L; force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_COMMAND_R_FORCE_ON) { modifierFlag = ModifierFlag::COMMAND_R; force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_L_FORCE_ON) { modifierFlag = ModifierFlag::CONTROL_L; force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_R_FORCE_ON) { modifierFlag = ModifierFlag::CONTROL_R; force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_FN_FORCE_ON)        { modifierFlag = ModifierFlag::FN;        force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_OPTION_L_FORCE_ON)  { modifierFlag = ModifierFlag::OPTION_L;  force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_OPTION_R_FORCE_ON)  { modifierFlag = ModifierFlag::OPTION_R;  force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_L_FORCE_ON)   { modifierFlag = ModifierFlag::SHIFT_L;   force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_R_FORCE_ON)   { modifierFlag = ModifierFlag::SHIFT_R;   force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA1_FORCE_ON)    { modifierFlag = ModifierFlag::EXTRA1;    force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA2_FORCE_ON)    { modifierFlag = ModifierFlag::EXTRA2;    force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA3_FORCE_ON)    { modifierFlag = ModifierFlag::EXTRA3;    force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA4_FORCE_ON)    { modifierFlag = ModifierFlag::EXTRA4;    force_on = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA5_FORCE_ON)    { modifierFlag = ModifierFlag::EXTRA5;    force_on = true; }

    else if (params.key == KeyCode::VK_LOCK_COMMAND_L_FORCE_OFF) { modifierFlag = ModifierFlag::COMMAND_L; force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_COMMAND_R_FORCE_OFF) { modifierFlag = ModifierFlag::COMMAND_R; force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_L_FORCE_OFF) { modifierFlag = ModifierFlag::CONTROL_L; force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_R_FORCE_OFF) { modifierFlag = ModifierFlag::CONTROL_R; force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_FN_FORCE_OFF)        { modifierFlag = ModifierFlag::FN;        force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_OPTION_L_FORCE_OFF)  { modifierFlag = ModifierFlag::OPTION_L;  force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_OPTION_R_FORCE_OFF)  { modifierFlag = ModifierFlag::OPTION_R;  force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_L_FORCE_OFF)   { modifierFlag = ModifierFlag::SHIFT_L;   force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_R_FORCE_OFF)   { modifierFlag = ModifierFlag::SHIFT_R;   force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA1_FORCE_OFF)    { modifierFlag = ModifierFlag::EXTRA1;    force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA2_FORCE_OFF)    { modifierFlag = ModifierFlag::EXTRA2;    force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA3_FORCE_OFF)    { modifierFlag = ModifierFlag::EXTRA3;    force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA4_FORCE_OFF)    { modifierFlag = ModifierFlag::EXTRA4;    force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA5_FORCE_OFF)    { modifierFlag = ModifierFlag::EXTRA5;    force_off = true; }

    else if (params.key == KeyCode::VK_LOCK_ALL_FORCE_OFF) {
      FlagStatus::globalFlagStatus().lock_clear();
      EventOutputQueue::FireModifiers::fire();
      return true;
    } else {
      return false;
    }

    if (params.ex_iskeydown && params.repeat == false) {
      if (force_off) {
        FlagStatus::globalFlagStatus().lock_decrease(modifierFlag);
      } else if (force_on) {
        FlagStatus::globalFlagStatus().lock_increase(modifierFlag);
      } else {
        FlagStatus::globalFlagStatus().lock_toggle(modifierFlag);
      }
      EventOutputQueue::FireModifiers::fire();
    }

    return true;
  }
}
