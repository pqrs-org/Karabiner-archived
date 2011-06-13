#include "base.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_STICKY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_STICKY::handle(const Params_KeyboardEventCallBack& params)
  {
    ModifierFlag flag = ModifierFlag::NONE;

    /**/ if (params.key == KeyCode::VK_STICKY_COMMAND_L) { flag = ModifierFlag::COMMAND_L; }
    else if (params.key == KeyCode::VK_STICKY_COMMAND_R) { flag = ModifierFlag::COMMAND_R; }
    else if (params.key == KeyCode::VK_STICKY_CONTROL_L) { flag = ModifierFlag::CONTROL_L; }
    else if (params.key == KeyCode::VK_STICKY_CONTROL_R) { flag = ModifierFlag::CONTROL_R; }
    else if (params.key == KeyCode::VK_STICKY_FN)        { flag = ModifierFlag::FN;        }
    else if (params.key == KeyCode::VK_STICKY_OPTION_L)  { flag = ModifierFlag::OPTION_L;  }
    else if (params.key == KeyCode::VK_STICKY_OPTION_R)  { flag = ModifierFlag::OPTION_R;  }
    else if (params.key == KeyCode::VK_STICKY_SHIFT_L)   { flag = ModifierFlag::SHIFT_L;   }
    else if (params.key == KeyCode::VK_STICKY_SHIFT_R)   { flag = ModifierFlag::SHIFT_R;   }
    else if (params.key == KeyCode::VK_STICKY_EXTRA1)    { flag = ModifierFlag::EXTRA1;    }
    else if (params.key == KeyCode::VK_STICKY_EXTRA2)    { flag = ModifierFlag::EXTRA2;    }
    else if (params.key == KeyCode::VK_STICKY_EXTRA3)    { flag = ModifierFlag::EXTRA3;    }
    else if (params.key == KeyCode::VK_STICKY_EXTRA4)    { flag = ModifierFlag::EXTRA4;    }
    else if (params.key == KeyCode::VK_STICKY_EXTRA5)    { flag = ModifierFlag::EXTRA5;    }
    else {
      return false;
    }

    if (params.ex_iskeydown && params.repeat == false) {
      FlagStatus::sticky_toggle(flag);
      EventOutputQueue::FireModifiers::fire();
    }

    return true;
  }
}
