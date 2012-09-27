#include "base.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_LOCK.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_LOCK::handle(const Params_KeyboardEventCallBack& params)
  {
    Flags flags(0);
    bool force_on = false;
    bool force_off = false;

    /**/ if (params.key == KeyCode::VK_LOCK_COMMAND_L)           { flags.add(ModifierFlag::COMMAND_L);                   }
    else if (params.key == KeyCode::VK_LOCK_COMMAND_R)           { flags.add(ModifierFlag::COMMAND_R);                   }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_L)           { flags.add(ModifierFlag::CONTROL_L);                   }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_R)           { flags.add(ModifierFlag::CONTROL_R);                   }
    else if (params.key == KeyCode::VK_LOCK_FN)                  { flags.add(ModifierFlag::FN);                          }
    else if (params.key == KeyCode::VK_LOCK_OPTION_L)            { flags.add(ModifierFlag::OPTION_L);                    }
    else if (params.key == KeyCode::VK_LOCK_OPTION_R)            { flags.add(ModifierFlag::OPTION_R);                    }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_L)             { flags.add(ModifierFlag::SHIFT_L);                     }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_R)             { flags.add(ModifierFlag::SHIFT_R);                     }
    else if (params.key == KeyCode::VK_LOCK_EXTRA1)              { flags.add(ModifierFlag::EXTRA1);                      }
    else if (params.key == KeyCode::VK_LOCK_EXTRA2)              { flags.add(ModifierFlag::EXTRA2);                      }
    else if (params.key == KeyCode::VK_LOCK_EXTRA3)              { flags.add(ModifierFlag::EXTRA3);                      }
    else if (params.key == KeyCode::VK_LOCK_EXTRA4)              { flags.add(ModifierFlag::EXTRA4);                      }
    else if (params.key == KeyCode::VK_LOCK_EXTRA5)              { flags.add(ModifierFlag::EXTRA5);                      }

    else if (params.key == KeyCode::VK_LOCK_COMMAND_L_FORCE_ON)  { flags.add(ModifierFlag::COMMAND_L); force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_COMMAND_R_FORCE_ON)  { flags.add(ModifierFlag::COMMAND_R); force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_L_FORCE_ON)  { flags.add(ModifierFlag::CONTROL_L); force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_R_FORCE_ON)  { flags.add(ModifierFlag::CONTROL_R); force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_FN_FORCE_ON)         { flags.add(ModifierFlag::FN);        force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_OPTION_L_FORCE_ON)   { flags.add(ModifierFlag::OPTION_L);  force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_OPTION_R_FORCE_ON)   { flags.add(ModifierFlag::OPTION_R);  force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_L_FORCE_ON)    { flags.add(ModifierFlag::SHIFT_L);   force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_R_FORCE_ON)    { flags.add(ModifierFlag::SHIFT_R);   force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_EXTRA1_FORCE_ON)     { flags.add(ModifierFlag::EXTRA1);    force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_EXTRA2_FORCE_ON)     { flags.add(ModifierFlag::EXTRA2);    force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_EXTRA3_FORCE_ON)     { flags.add(ModifierFlag::EXTRA3);    force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_EXTRA4_FORCE_ON)     { flags.add(ModifierFlag::EXTRA4);    force_on = true;  }
    else if (params.key == KeyCode::VK_LOCK_EXTRA5_FORCE_ON)     { flags.add(ModifierFlag::EXTRA5);    force_on = true;  }

    else if (params.key == KeyCode::VK_LOCK_COMMAND_L_FORCE_OFF) { flags.add(ModifierFlag::COMMAND_L); force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_COMMAND_R_FORCE_OFF) { flags.add(ModifierFlag::COMMAND_R); force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_L_FORCE_OFF) { flags.add(ModifierFlag::CONTROL_L); force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_CONTROL_R_FORCE_OFF) { flags.add(ModifierFlag::CONTROL_R); force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_FN_FORCE_OFF)        { flags.add(ModifierFlag::FN);        force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_OPTION_L_FORCE_OFF)  { flags.add(ModifierFlag::OPTION_L);  force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_OPTION_R_FORCE_OFF)  { flags.add(ModifierFlag::OPTION_R);  force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_L_FORCE_OFF)   { flags.add(ModifierFlag::SHIFT_L);   force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_SHIFT_R_FORCE_OFF)   { flags.add(ModifierFlag::SHIFT_R);   force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA1_FORCE_OFF)    { flags.add(ModifierFlag::EXTRA1);    force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA2_FORCE_OFF)    { flags.add(ModifierFlag::EXTRA2);    force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA3_FORCE_OFF)    { flags.add(ModifierFlag::EXTRA3);    force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA4_FORCE_OFF)    { flags.add(ModifierFlag::EXTRA4);    force_off = true; }
    else if (params.key == KeyCode::VK_LOCK_EXTRA5_FORCE_OFF)    { flags.add(ModifierFlag::EXTRA5);    force_off = true; }

    else if (params.key == KeyCode::VK_LOCK_ALL_FORCE_OFF) {
      FlagStatus::lock_clear();
      EventOutputQueue::FireModifiers::fire();
      return true;
    } else {
      return false;
    }

    if (params.ex_iskeydown && params.repeat == false) {
      if (force_off) {
        FlagStatus::lock_decrease(flags);
      } else if (force_on) {
        FlagStatus::lock_increase(flags);
      } else {
        FlagStatus::lock_toggle(flags);
      }
      EventOutputQueue::FireModifiers::fire();
    }

    return true;
  }
}
