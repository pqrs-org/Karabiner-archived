#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "PressingPhysicalKeys.hpp"

namespace org_pqrs_Karabiner {
void PressingPhysicalKeys::update(const Params_Base &paramsBase) {
  bool iskeydown;
  if (!paramsBase.iskeydown(iskeydown)) return;

  // ------------------------------------------------------------
  // When we press&release CapsLock, key event is fired only once.
  // (down or up depending on the state of CapsLock)
  // If we use Virtual CapsLock (remapped CapsLock) like "Change A to CapsLock",
  // the PressingPhysicalKeys state is increase illegally.
  // So, we ignore Hardware CapsLock at PressingPhysicalKeys.
  //
  // (1) Press Hardware CapsLock (EventType::DOWN is fired.)
  // (2) Press A (EventType::DOWN is fired.)
  // (2') (A is changed to CapsLock.)
  // (3) Release A (EventType::UP is fired.)
  // (3') (A is changed to CapsLock.)
  // (4) Press Hardware CapsLock (EventType::DOWN is fired.)
  //
  // Both (1) and (4) fire DOWN event.
  {
    auto params = paramsBase.get_Params_KeyboardEventCallBack();
    if (params && params->key == KeyCode::CAPSLOCK) {
      return;
    }
  }

  // ------------------------------------------------------------
  if (iskeydown) {
    list_.push_back(new Item(paramsBase));

  } else {
    auto p = static_cast<Item *>(list_.safe_front());
    for (;;) {
      if (!p) break;

      if ((p->fromEvent).isTargetEvent(paramsBase)) {
        p = static_cast<Item *>(list_.erase_and_delete(p));

        if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_allow_devices_multiple_same_key)) {
          break;
        }

      } else {
        p = static_cast<Item *>(p->getnext());
      }
    }
  }
}

void PressingPhysicalKeys::clear(void) {
  list_.clear();
}

bool PressingPhysicalKeys::empty(void) const {
  return list_.size() == 0;
}

size_t PressingPhysicalKeys::count(void) const {
  return list_.size();
}
}
