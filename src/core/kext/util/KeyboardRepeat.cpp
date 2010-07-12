#include "CommonData.hpp"
#include "EventOutputQueue.hpp"
#include "IOLockWrapper.hpp"
#include "KeyboardRepeat.hpp"
#include "ListHookedKeyboard.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* KeyboardRepeat::queue_ = NULL;
  TimerWrapper KeyboardRepeat::timer_;

  void
  KeyboardRepeat::initialize(IOWorkLoop& workloop)
  {
    queue_ = new List();
    timer_.initialize(&workloop, NULL, KeyboardRepeat::fire);
  }

  void
  KeyboardRepeat::terminate(void)
  {
    timer_.terminate();

    if (queue_) {
      delete queue_;
    }
  }

  void
  KeyboardRepeat::cancel_nolock(void)
  {
    timer_.cancelTimeout();
    if (queue_) {
      queue_->clear();
    }
  }

  void
  KeyboardRepeat::cancel(void)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    IOLOG_DEVEL("KeyboardRepeat::cancel\n");
    cancel_nolock();
  }

  void
  KeyboardRepeat::primitive_add_nolock(EventType eventType,
                                       Flags flags,
                                       KeyCode key,
                                       KeyboardType keyboardType)
  {
    if (! queue_) return;
    if (key == KeyCode::VK_NONE) return;

    // ------------------------------------------------------------
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(eventType,
                                                                                   flags,
                                                                                   key,
                                                                                   keyboardType,
                                                                                   true));
    if (! ptr) return;
    Params_KeyboardEventCallBack& params = *ptr;
    queue_->push_back(new Item(params));
  }

  void
  KeyboardRepeat::primitive_add_nolock(EventType eventType,
                                       Flags flags,
                                       ConsumerKeyCode key)
  {
    if (! queue_) return;
    if (key == ConsumerKeyCode::VK_NONE) return;

    // ------------------------------------------------------------
    Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(eventType,
                                                                                                 flags,
                                                                                                 key,
                                                                                                 true));
    if (! ptr) return;
    Params_KeyboardSpecialEventCallback& params = *ptr;
    queue_->push_back(new Item(params));
  }

  void
  KeyboardRepeat::primitive_add(EventType eventType,
                                Flags flags,
                                KeyCode key,
                                KeyboardType keyboardType)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    primitive_add_nolock(eventType, flags, key, keyboardType);
  }

  void
  KeyboardRepeat::primitive_add(EventType eventType,
                                Flags flags,
                                ConsumerKeyCode key)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    primitive_add_nolock(eventType, flags, key);
  }

  void
  KeyboardRepeat::primitive_start_nolock(int wait)
  {
    timer_.setTimeoutMS(wait);
  }

  void
  KeyboardRepeat::primitive_start(int wait)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    primitive_start_nolock(wait);
  }

  void
  KeyboardRepeat::set(EventType eventType,
                      Flags flags,
                      KeyCode key,
                      KeyboardType keyboardType,
                      int wait)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    if (eventType == EventType::MODIFY) {
      goto cancel;

    } else if (eventType == EventType::UP) {
      // We always stop key repeat at key up when multiple keys repeat.
      if (queue_->size() != 1) goto cancel;

      // We stop key repeat only when the repeating key is up.
      KeyboardRepeat::Item* p = static_cast<KeyboardRepeat::Item*>(queue_->front());
      if (p && (p->params).type == ParamsUnion::KEYBOARD) {
        Params_KeyboardEventCallBack* params = (p->params).params.params_KeyboardEventCallBack;
        if (params && key == params->key) {
          goto cancel;
        }
      }

    } else if (eventType == EventType::DOWN) {
      if (key == KeyCode::VK_NONE) goto cancel;

      cancel_nolock();

      primitive_add_nolock(eventType, flags, key, keyboardType);
      primitive_start_nolock(wait);

      if (config.debug_devel) {
        IOLOG_INFO("KeyboardRepeat::set key:%d flags:0x%x\n", key.get(), flags.get());
      }

    } else {
      goto cancel;
    }

    return;

  cancel:
    cancel_nolock();
  }

  void
  KeyboardRepeat::set(EventType eventType,
                      Flags flags,
                      ConsumerKeyCode key)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    if (eventType == EventType::UP) {
      goto cancel;

    } else if (eventType == EventType::DOWN) {
      if (key != ConsumerKeyCode::BRIGHTNESS_DOWN &&
          key != ConsumerKeyCode::BRIGHTNESS_UP &&
          key != ConsumerKeyCode::KEYBOARDLIGHT_LOW &&
          key != ConsumerKeyCode::KEYBOARDLIGHT_HIGH &&
          key != ConsumerKeyCode::MUSIC_PREV &&
          key != ConsumerKeyCode::MUSIC_NEXT &&
          key != ConsumerKeyCode::VOLUME_DOWN &&
          key != ConsumerKeyCode::VOLUME_UP) {
        goto cancel;
      }

      cancel_nolock();

      primitive_add_nolock(eventType, flags, key);
      primitive_start_nolock(config.get_repeat_consumer_initial_wait());

      if (config.debug_devel) {
        IOLOG_INFO("KeyboardRepeat::set consumer key:%d flags:0x%x\n", key.get(), flags.get());
      }

    } else {
      goto cancel;
    }

    return;

  cancel:
    cancel_nolock();
  }

  void
  KeyboardRepeat::fire(OSObject* owner, IOTimerEventSource* sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    IOLOG_DEVEL("KeyboardRepeat::fire queue_->size = %d\n", static_cast<int>(queue_->size()));

    // ----------------------------------------
    bool isconsumer = false;

    for (KeyboardRepeat::Item* p = static_cast<KeyboardRepeat::Item*>(queue_->front()); p; p = static_cast<KeyboardRepeat::Item*>(p->getnext())) {
      switch ((p->params).type) {
        case ParamsUnion::KEYBOARD:
        {
          Params_KeyboardEventCallBack* params = (p->params).params.params_KeyboardEventCallBack;
          if (params) {
            Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params->eventType,
                                                                                           params->flags,
                                                                                           params->key,
                                                                                           params->keyboardType,
                                                                                           queue_->size() == 1 ? true : false));
            if (ptr) {
              EventOutputQueue::FireKey::fire(*ptr);
            }
          }
          break;
        }

        case ParamsUnion::KEYBOARD_SPECIAL:
        {
          Params_KeyboardSpecialEventCallback* params = (p->params).params.params_KeyboardSpecialEventCallback;
          if (params) {
            Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(params->eventType,
                                                                                                         params->flags,
                                                                                                         params->key,
                                                                                                         queue_->size() == 1 ? true : false));
            if (ptr) {
              EventOutputQueue::FireConsumer::fire(*ptr);
            }
          }
          isconsumer = true;
          break;
        }

        case ParamsUnion::UPDATE_FLAGS:
        case ParamsUnion::RELATIVE_POINTER:
        case ParamsUnion::SCROLL_POINTER:
          // do nothing
          break;
      }
    }

    if (isconsumer) {
      timer_.setTimeoutMS(config.get_repeat_consumer_wait());
    } else {
      timer_.setTimeoutMS(config.get_repeat_wait());
    }
  }
}
