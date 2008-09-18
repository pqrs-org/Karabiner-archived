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
  ListFireExtraKey::fire(KeyboardEventCallback callback, const Params_KeyboardEventCallBack &params)
  {
    if (callback == NULL) return;

    Params_KeyboardEventCallBack callbackparams = params;

    for (int i = 0; i < size; ++i) {
      Item &item = list[i];

      callbackparams.eventType = item.getEventType();
      callbackparams.flags = item.getFlags();
      callbackparams.key = item.getKey();

      RemapUtil::fireKey(callback, callbackparams);
    }
  }
}
