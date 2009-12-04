#include "PressDownKeys.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"

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
  PressDownKeys::add(unsigned int key, const KeyboardType& keyboardType)
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
          printf("KeyRemap4MacBook -Info- PressDownKeys::add (key = %d)\n", key);
        }
        return;
      }
    }
  }

  void
  PressDownKeys::remove(unsigned int key, const KeyboardType& keyboardType)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].enable) continue;
      if (item_[i].key != key) continue;
      if (item_[i].keyboardType != keyboardType) continue;

      item_[i].enable = false;

      if (config.debug_devel) {
        printf("KeyRemap4MacBook -Info- PressDownKeys::remove (key = %d)\n", key);
      }
    }
  }

  void
  PressDownKeys::clear(OSObject* target, AbsoluteTime ts, OSObject* sender, void* refcon)
  {
    if (config.debug_devel) {
      printf("KeyRemap4MacBook -Info- PressDownKeys::clear\n");
    }

    Params_KeyboardEventCallBack callbackparams = {
      target, KeyEvent::UP, 0, 0, 0, 0, 0, 0, 0, false, ts, sender, refcon,
    };

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].enable) continue;
      item_[i].enable = false;

      if (config.debug_devel) {
        printf("KeyRemap4MacBook -Info- PressDownKeys::clear (key = %d)\n", item_[i].key);
      }

      callbackparams.key = item_[i].key;
      callbackparams.keyboardType = item_[i].keyboardType;
      callbackparams.apply();
    }
  }
}
