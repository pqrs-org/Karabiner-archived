#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_STICKY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_STICKY::handle(const Params_KeyboardEventCallBack& params)
  {
    ModifierFlag flag = ModifierFlag::NONE;
    bool force_on = false;
    bool force_off = false;

    /**/ if (params.key == KeyCode::VK_STICKY_COMMAND_L)           { flag = ModifierFlag::COMMAND_L; }
    else if (params.key == KeyCode::VK_STICKY_COMMAND_R)           { flag = ModifierFlag::COMMAND_R; }
    else if (params.key == KeyCode::VK_STICKY_CONTROL_L)           { flag = ModifierFlag::CONTROL_L; }
    else if (params.key == KeyCode::VK_STICKY_CONTROL_R)           { flag = ModifierFlag::CONTROL_R; }
    else if (params.key == KeyCode::VK_STICKY_FN)                  { flag = ModifierFlag::FN;        }
    else if (params.key == KeyCode::VK_STICKY_OPTION_L)            { flag = ModifierFlag::OPTION_L;  }
    else if (params.key == KeyCode::VK_STICKY_OPTION_R)            { flag = ModifierFlag::OPTION_R;  }
    else if (params.key == KeyCode::VK_STICKY_SHIFT_L)             { flag = ModifierFlag::SHIFT_L;   }
    else if (params.key == KeyCode::VK_STICKY_SHIFT_R)             { flag = ModifierFlag::SHIFT_R;   }
    else if (params.key == KeyCode::VK_STICKY_EXTRA1)              { flag = ModifierFlag::EXTRA1;    }
    else if (params.key == KeyCode::VK_STICKY_EXTRA2)              { flag = ModifierFlag::EXTRA2;    }
    else if (params.key == KeyCode::VK_STICKY_EXTRA3)              { flag = ModifierFlag::EXTRA3;    }
    else if (params.key == KeyCode::VK_STICKY_EXTRA4)              { flag = ModifierFlag::EXTRA4;    }
    else if (params.key == KeyCode::VK_STICKY_EXTRA5)              { flag = ModifierFlag::EXTRA5;    }

    else if (params.key == KeyCode::VK_STICKY_COMMAND_L_FORCE_ON)  { flag = ModifierFlag::COMMAND_L; force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_COMMAND_R_FORCE_ON)  { flag = ModifierFlag::COMMAND_R; force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_CONTROL_L_FORCE_ON)  { flag = ModifierFlag::CONTROL_L; force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_CONTROL_R_FORCE_ON)  { flag = ModifierFlag::CONTROL_R; force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_FN_FORCE_ON)         { flag = ModifierFlag::FN;        force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_OPTION_L_FORCE_ON)   { flag = ModifierFlag::OPTION_L;  force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_OPTION_R_FORCE_ON)   { flag = ModifierFlag::OPTION_R;  force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_SHIFT_L_FORCE_ON)    { flag = ModifierFlag::SHIFT_L;   force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_SHIFT_R_FORCE_ON)    { flag = ModifierFlag::SHIFT_R;   force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA1_FORCE_ON)     { flag = ModifierFlag::EXTRA1;    force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA2_FORCE_ON)     { flag = ModifierFlag::EXTRA2;    force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA3_FORCE_ON)     { flag = ModifierFlag::EXTRA3;    force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA4_FORCE_ON)     { flag = ModifierFlag::EXTRA4;    force_on = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA5_FORCE_ON)     { flag = ModifierFlag::EXTRA5;    force_on = true; }

    else if (params.key == KeyCode::VK_STICKY_COMMAND_L_FORCE_OFF) { flag = ModifierFlag::COMMAND_L; force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_COMMAND_R_FORCE_OFF) { flag = ModifierFlag::COMMAND_R; force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_CONTROL_L_FORCE_OFF) { flag = ModifierFlag::CONTROL_L; force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_CONTROL_R_FORCE_OFF) { flag = ModifierFlag::CONTROL_R; force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_FN_FORCE_OFF)        { flag = ModifierFlag::FN;        force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_OPTION_L_FORCE_OFF)  { flag = ModifierFlag::OPTION_L;  force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_OPTION_R_FORCE_OFF)  { flag = ModifierFlag::OPTION_R;  force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_SHIFT_L_FORCE_OFF)   { flag = ModifierFlag::SHIFT_L;   force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_SHIFT_R_FORCE_OFF)   { flag = ModifierFlag::SHIFT_R;   force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA1_FORCE_OFF)    { flag = ModifierFlag::EXTRA1;    force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA2_FORCE_OFF)    { flag = ModifierFlag::EXTRA2;    force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA3_FORCE_OFF)    { flag = ModifierFlag::EXTRA3;    force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA4_FORCE_OFF)    { flag = ModifierFlag::EXTRA4;    force_off = true; }
    else if (params.key == KeyCode::VK_STICKY_EXTRA5_FORCE_OFF)    { flag = ModifierFlag::EXTRA5;    force_off = true; }

    else {
      return false;
    }

    if (params.ex_iskeydown && params.repeat == false) {
      if (force_off) {
        FlagStatus::sticky_decrease(flag);
      } else if (force_on) {
        FlagStatus::sticky_increase(flag);
      } else {
        FlagStatus::sticky_toggle(flag);
      }
      EventOutputQueue::FireModifiers::fire();
    }

    return true;
  }
}
