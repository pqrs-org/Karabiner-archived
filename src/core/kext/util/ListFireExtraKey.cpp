#include "listFireExtraKey.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  ListFireExtraKey::addKey(unsigned int flags, KeyCode::KeyCode keyCode)
  {
    if (RemapUtil::getKeyCodeModifier(keyCode) != ModifierFlag::NONE) {
      add(KeyEvent::MODIFY, flags, keyCode);
    } else {
      add(KeyEvent::DOWN, flags, keyCode);
      add(KeyEvent::UP, flags, keyCode);
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

      RemapUtil::fireKey(callback, target, item.getEventType(), item.getFlags(), item.getKey(),
                         0, 0, 0, 0,
                         keyboardType, false, ts, sender, refcon);
    }
  }
}
