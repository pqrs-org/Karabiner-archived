#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "PressingPhysicalKeys.hpp"

namespace org_pqrs_Karabiner {
List PressingPhysicalKeys::list_;

void PressingPhysicalKeys::initialize(void) {}

void PressingPhysicalKeys::terminate(void) {
  list_.clear();
}

void PressingPhysicalKeys::update(const Params_Base &paramsBase) {
  bool iskeydown;
  if (!paramsBase.iskeydown(iskeydown)) return;

  if (iskeydown) {
    list_.push_back(new Item(paramsBase));

  } else {
    auto p = static_cast<Item *>(list_.safe_front());
    for (;;) {
      if (!p) break;

      // We remove only one matched item. (keep other matched items.)
      // Because, if keyboard has same key (by Seil or multiple keyboards are connected),
      // the same key entries might exist in list_.
      if ((p->fromEvent).isTargetEvent(paramsBase)) {
        p = static_cast<Item *>(list_.erase_and_delete(p));

        if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_workaround_stuck_keyboards)) {
          // Some keyboards sometimes drop keyup events for modifiers and modifiers will be stuck.
          // So, remove all matched items.
          continue;
        }

        return;

      } else {
        p = static_cast<Item *>(p->getnext());
      }
    }
  }
}

void PressingPhysicalKeys::clear(void) {
  list_.clear();
}

bool PressingPhysicalKeys::empty(void) {
  return list_.size() == 0;
}

size_t PressingPhysicalKeys::count(void) {
  return list_.size();
}
}
