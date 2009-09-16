#include "PressDownKeys.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace PressDownKeys {
    enum {
      MAXNUM = 16,
    };
    struct Item {
      bool enable;
      unsigned int key;
      unsigned int keyboardType;
    };
    Item item[MAXNUM];

    void
    initialize(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        item[i].enable = false;
      }
    }

    void
    add(unsigned int _key, unsigned int _keyboardType)
    {
      // skip if already added.
      for (int i = 0; i < MAXNUM; ++i) {
        if (! item[i].enable) continue;
        if (item[i].key != _key) continue;
        if (item[i].keyboardType != _keyboardType) continue;
        return;
      }

      // register
      for (int i = 0; i < MAXNUM; ++i) {
        if (! item[i].enable) {
          item[i].enable = true;
          item[i].key = _key;
          item[i].keyboardType = _keyboardType;

          if (config.debug_devel) {
            printf("KeyRemap4MacBook -Info- PressDownKeys::add (key = %d)\n", _key);
          }
          return;
        }
      }
    }

    void
    remove(unsigned int _key, unsigned int _keyboardType)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        if (! item[i].enable) continue;
        if (item[i].key != _key) continue;
        if (item[i].keyboardType != _keyboardType) continue;
        item[i].enable = false;

        if (config.debug_devel) {
          printf("KeyRemap4MacBook -Info- PressDownKeys::remove (key = %d)\n", _key);
        }
      }
    }

    void
    clear(KeyboardEventCallback callback, OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon)
    {
      if (config.debug_devel) {
        printf("KeyRemap4MacBook -Info- PressDownKeys::clear\n");
      }

      if (callback == NULL) return;

      Params_KeyboardEventCallBack callbackparams = {
        target, KeyEvent::UP, 0, 0, 0, 0, 0, 0, 0, false, ts, sender, refcon,
      };

      for (int i = 0; i < MAXNUM; ++i) {
        if (! item[i].enable) continue;
        item[i].enable = false;

        if (config.debug_devel) {
          printf("KeyRemap4MacBook -Info- PressDownKeys::clear (key = %d)\n", item[i].key);
        }

        callbackparams.key = item[i].key;
        callbackparams.keyboardType = item[i].keyboardType;
        callbackparams.apply(callback);
      }
    }
  }
}
