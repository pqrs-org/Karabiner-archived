#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "GlobalLock.hpp"
#include "KeyCode.hpp"
#include "ListHookedKeyboard.hpp"
#include "Params.hpp"
#include "VK_IOHIKEYBOARD_TOGGLE_NUMLOCK.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_IOHIKEYBOARD_TOGGLE_NUMLOCK::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  if (params.key != KeyCode::VK_IOHIKEYBOARD_TOGGLE_NUMLOCK) return false;

  if (params.ex_iskeydown && params.repeat == false) {
    ListHookedKeyboard::Item* item = static_cast<ListHookedKeyboard::Item*>(ListHookedKeyboard::instance().get_replaced());
    if (item) {
      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, item->get());
      if (kbd) {
        GlobalLock::ScopedUnlock lk;
        kbd->setNumLock(!kbd->numLock());
      }
    }
  }

  return true;
}
}
