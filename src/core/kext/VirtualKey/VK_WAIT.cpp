#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "VK_WAIT.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_WAIT::handle(const Params_KeyboardEventCallBack& params)
  {
    int milliseconds = 0;

    /**/ if (params.key == KeyCode::VK_WAIT_10MS)  { milliseconds = 10; }
    else if (params.key == KeyCode::VK_WAIT_50MS)  { milliseconds = 50; }
    else if (params.key == KeyCode::VK_WAIT_100MS) { milliseconds = 100; }

    if (milliseconds == 0) {
      return false;
    }

    Params_Wait::auto_ptr ptr(Params_Wait::alloc(milliseconds));
    if (ptr) {
      EventOutputQueue::FireWait::fire(*ptr);
    }
    return true;
  }
}
