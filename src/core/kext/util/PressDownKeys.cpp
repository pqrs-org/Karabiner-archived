#include "CallBackWrapper.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "PressDownKeys.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* PressDownKeys::list_;
  IOLock* PressDownKeys::lock_;

  void
  PressDownKeys::initialize(void)
  {
    lock_ = IOLockWrapper::alloc();
    list_ = new List();
  }

  void
  PressDownKeys::terminate(void)
  {
    IOLockWrapper::free(lock_);
    if (list_) {
      delete list_;
    }
  }

  void
  PressDownKeys::add(KeyCode key, KeyboardType keyboardType)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (! list_) return;

    if (key == KeyCode::VK_NONE) return;

    list_->push_back(new Item(key, keyboardType));

    IOLOG_DEVEL("PressDownKeys::add key:%d, keyboardType:%d\n", key.get(), keyboardType.get());
  }

  void
  PressDownKeys::remove(KeyCode key, KeyboardType keyboardType)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (! list_) return;

    Item* p = static_cast<Item*>(list_->front());
    for (;;) {
      if (! p) break;

      if (p->key == key &&
          p->keyboardType == keyboardType) {
        p = static_cast<Item*>(list_->erase(p));
      } else {
        p = static_cast<Item*>(p->getnext());
      }
    }

    IOLOG_DEVEL("PressDownKeys::remove key:%d, keyboardType:%d\n", key.get(), keyboardType.get());
  }

  void
  PressDownKeys::clear(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (! list_) return;

    IOLOG_DEVEL("PressDownKeys::clear\n");

    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::UP, Flags(0), KeyCode(0), CommonData::getcurrent_keyboardType(), false));
    if (! ptr) return;
    Params_KeyboardEventCallBack& params = *ptr;

    Item* p = static_cast<Item*>(list_->front());
    for (;;) {
      if (! p) break;

      params.key = p->key;
      params.keyboardType = p->keyboardType;
      EventOutputQueue::push(params);

      IOLOG_DEVEL("PressDownKeys::clear key:%d, keyboardType:%d\n", params.key.get(), params.keyboardType.get());

      p = static_cast<Item*>(list_->erase(p));
    }
  }
}
