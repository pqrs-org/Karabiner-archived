#include "listFireExtraKey.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  ListFireExtraKey::addKey(unsigned int flags, KeyCode::KeyCode keyCode, CharCode::CharCode charCode)
  {
    if (RemapUtil::getKeyCodeModifier(keyCode) != ModifierFlag::NONE) {
      add(KeyEvent::MODIFY, flags, keyCode, charCode);
    } else {
      add(KeyEvent::DOWN, flags, keyCode, charCode);
      add(KeyEvent::UP, flags, keyCode, charCode);
    }
  }

  void
  ListFireExtraKey::fire(KeyboardEventCallback callback,
                         OSObject *target,
                         unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet, unsigned int keyboardType,
                         AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    for (int i = 0; i < size; ++i) {
      Item &item = list[i];

      RemapUtil::fireKey(callback, target, item.getEventType(), item.getFlags(), item.getKey(), item.getCharCode(),
                         charSet, origCharCode, origCharSet,
                         keyboardType, false, ts, sender, refcon);
    }
  }
}
