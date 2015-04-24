#include "CommonData.hpp"
#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "Params.hpp"
#include "PressDownKeys.hpp"

namespace org_pqrs_Karabiner {
List PressDownKeys::list_;

void PressDownKeys::initialize(void) {}

void PressDownKeys::terminate(void) {
  list_.clear();
}

void PressDownKeys::add(KeyCode key, KeyboardType keyboardType) {
  if (key == KeyCode::VK_NONE) return;

  list_.push_back(new Item(key, keyboardType));

  IOLOG_DEVEL("PressDownKeys::add key:%d, keyboardType:%d\n", key.get(), keyboardType.get());
}

void PressDownKeys::remove(KeyCode key, KeyboardType keyboardType) {
  Item* p = static_cast<Item*>(list_.safe_front());
  for (;;) {
    if (!p) break;

    if (p->key == key &&
        p->keyboardType == keyboardType) {
      p = static_cast<Item*>(list_.erase_and_delete(p));
    } else {
      p = static_cast<Item*>(p->getnext());
    }
  }

  IOLOG_DEVEL("PressDownKeys::remove key:%d, keyboardType:%d\n", key.get(), keyboardType.get());
}

void PressDownKeys::clear(void) {
  IOLOG_DEVEL("PressDownKeys::clear list_.size = %d\n", static_cast<int>(list_.size()));

  Item* p = static_cast<Item*>(list_.safe_front());
  for (;;) {
    if (!p) break;

    Params_KeyboardEventCallBack params(EventType::UP, Flags(0), p->key, p->keyboardType, false);

    EventOutputQueue::push(params, AutogenId::maxValue());
    IOLOG_DEVEL("PressDownKeys::clear key:%d, keyboardType:%d\n", params.key.get(), params.keyboardType.get());

    p = static_cast<Item*>(list_.erase_and_delete(p));
  }
}
}
