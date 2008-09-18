#include "PressDownKeys.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  PressDownKeys::initialize(void)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item[i].enable = false;
    }
  }

  void
  PressDownKeys::add(unsigned int _key, unsigned int _keyboardType)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      if (! item[i].enable) {
        item[i].enable = true;
        item[i].key = _key;
        item[i].keyboardType = _keyboardType;
      }
    }
  }

  void
  PressDownKeys::remove(unsigned int _key, unsigned int _keyboardType)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      if (! item[i].enable) continue;
      if (item[i].key != _key) continue;
      if (item[i].keyboardType != _keyboardType) continue;
      item[i].enable = false;
    }
  }

  void
  PressDownKeys::clear(KeyboardEventCallback callback, OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    Params_KeyboardEventCallBack callbackparams = {
      target, KeyEvent::UP, 0, 0, 0, 0, 0, 0, 0, false, ts, sender, refcon,
    };

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item[i].enable) continue;
      item[i].enable = false;

      callbackparams.key = item[i].key;
      callbackparams.keyboardType = item[i].keyboardType;
      callbackparams.apply(callback);
    }
  }
}
