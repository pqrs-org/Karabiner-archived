#include "PressDownKeys.hpp"
#include "RemapUtil.hpp"
#include "keycode.hpp"

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

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item[i].enable) continue;
      item[i].enable = false;

      unsigned int flags = 0;
      unsigned int charCode = 0;
      unsigned int charSet = 0;
      unsigned int origCharCode = 0;
      unsigned int origCharSet = 0;
      RemapUtil::fireKey(callback,
                         target, KeyEvent::UP, flags, item[i].key,
                         charCode, charSet, origCharCode, origCharSet,
                         item[i].keyboardType, false, ts, sender, refcon);
    }
  }
}
