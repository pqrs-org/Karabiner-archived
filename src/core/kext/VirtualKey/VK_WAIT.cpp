#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "VK_WAIT.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_WAIT::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId) {
  int milliseconds = 0;

  /**/ if (params.key == KeyCode::VK_WAIT_10MS) {
    milliseconds = 10;
  } else if (params.key == KeyCode::VK_WAIT_50MS) {
    milliseconds = 50;
  } else if (params.key == KeyCode::VK_WAIT_100MS) {
    milliseconds = 100;
  }

  if (milliseconds == 0) {
    return false;
  }

  Params_Wait p(milliseconds);
  EventOutputQueue::FireWait::fire(p, autogenId);

  return true;
}
}
