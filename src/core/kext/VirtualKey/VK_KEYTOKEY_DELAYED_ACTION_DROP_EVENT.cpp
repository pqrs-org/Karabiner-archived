#include <IOKit/IOLib.h>

#include "Params.hpp"
#include "VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT::needToDrop_ = false;

void VirtualKey::VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT::reset(void) {
  needToDrop_ = false;
}

bool VirtualKey::VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT::handle(const Params_KeyboardEventCallBack& params) {
  if (params.key == KeyCode::VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT) {
    return true;
  }

  return false;
}
}
