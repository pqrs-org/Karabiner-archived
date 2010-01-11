#include "CommonData.hpp"
#include "Config.hpp"
#include "PressDownKeys.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  PressDownKeys::Item PressDownKeys::item_[PressDownKeys::MAXNUM];

  void
  PressDownKeys::initialize(void)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].enable = false;
    }
  }

  void
  PressDownKeys::add(const KeyCode& key, const KeyboardType& keyboardType)
  {
    if (key == KeyCode::VK_NONE) return;

    // skip if already added.
    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].enable) continue;
      if (item_[i].key != key) continue;
      if (item_[i].keyboardType != keyboardType) continue;
      return;
    }

    // register
    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].enable) {
        item_[i].enable = true;
        item_[i].key = key;
        item_[i].keyboardType = keyboardType;

        if (config.debug_devel) {
          IOLog("KeyRemap4MacBook -Info- PressDownKeys::add (key = %d)\n", key.get());
        }
        return;
      }
    }
  }

  void
  PressDownKeys::remove(const KeyCode& key, const KeyboardType& keyboardType)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].enable) continue;
      if (item_[i].key != key) continue;
      if (item_[i].keyboardType != keyboardType) continue;

      item_[i].enable = false;

      if (config.debug_devel) {
        IOLog("KeyRemap4MacBook -Info- PressDownKeys::remove (key = %d)\n", key.get());
      }
    }
  }

  void
  PressDownKeys::clear(void)
  {
    if (config.debug_devel) {
      IOLog("KeyRemap4MacBook -Info- PressDownKeys::clear\n");
    }

    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::UP, Flags(0), KeyCode(0), CommonData::getcurrent_keyboardType(), false));
    if (! ptr) return;
    Params_KeyboardEventCallBack& params = *ptr;

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].enable) continue;
      item_[i].enable = false;

      if (config.debug_devel) {
        IOLog("KeyRemap4MacBook -Info- PressDownKeys::clear (key = %d)\n", item_[i].key.get());
      }

      params.key = item_[i].key;
      params.keyboardType = item_[i].keyboardType;
      params.apply();
    }
  }
}
