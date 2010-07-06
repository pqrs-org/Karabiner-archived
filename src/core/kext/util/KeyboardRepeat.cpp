#include "CommonData.hpp"
#include "EventOutputQueue.hpp"
#include "IOLockWrapper.hpp"
#include "KeyboardRepeat.hpp"
#include "ListHookedKeyboard.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Queue* KeyboardRepeat::queue_ = NULL;
  TimerWrapper KeyboardRepeat::timer_;

  void
  KeyboardRepeat::initialize(IOWorkLoop& workloop)
  {
    queue_ = new Queue();
    timer_.initialize(&workloop, NULL, KeyboardRepeat::fire);
  }

  void
  KeyboardRepeat::terminate(void)
  {
    timer_.terminate();

    clear_queue();
    if (queue_) {
      delete queue_;
    }
  }

  KeyboardRepeat::Item::~Item(void)
  {
#define DELETE_PARAMS(PARAMS) { \
    if (PARAMS) {               \
      delete PARAMS;            \
    }                           \
}

    switch (type) {
      case TYPE_KEYBOARD:
        DELETE_PARAMS(params.params_KeyboardEventCallBack);
        break;

      case TYPE_CONSUMER:
        DELETE_PARAMS(params.params_KeyboardSpecialEventCallback);
        break;
    }

#undef DELETE_PARAMS
  }

  void
  KeyboardRepeat::clear_queue(void)
  {
    if (! queue_) return;

    for (;;) {
      KeyboardRepeat::Item* p = static_cast<KeyboardRepeat::Item*>(queue_->front());
      if (! p) break;

      queue_->pop();
      delete p;
    }
  }

  void
  KeyboardRepeat::cancel_nolock(void)
  {
    timer_.cancelTimeout();
    clear_queue();
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
    Item* newp = new Item();
    if (! newp) return;

    newp->type = Item::TYPE_KEYBOARD;
    newp->params.params_KeyboardEventCallBack = Params_KeyboardEventCallBack::alloc(eventType,
                                                                                    flags,
                                                                                    key,
                                                                                    keyboardType,
                                                                                    true);
    queue_->push(newp);
  }

  void
  KeyboardRepeat::primitive_add_nolock(EventType eventType,
                                       Flags flags,
                                       ConsumerKeyCode key)
  {
    if (! queue_) return;
    if (key == ConsumerKeyCode::VK_NONE) return;

    // ------------------------------------------------------------
    Item* newp = new Item();
    if (! newp) return;

    newp->type = Item::TYPE_CONSUMER;
    newp->params.params_KeyboardSpecialEventCallback = Params_KeyboardSpecialEventCallback::alloc(eventType,
                                                                                                  flags,
                                                                                                  key,
                                                                                                  true);
    queue_->push(newp);
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
      if (p && p->type == Item::TYPE_KEYBOARD) {
        Params_KeyboardEventCallBack* params = p->params.params_KeyboardEventCallBack;
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
    if (! CommonData::eventLock) return;
    IOLockWrapper::ScopedLock eventLock(CommonData::eventLock);

    // ------------------------------------------------------------
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    // ----------------------------------------
    bool isconsumer = false;

    for (KeyboardRepeat::Item* p = static_cast<KeyboardRepeat::Item*>(queue_->front()); p; p = static_cast<KeyboardRepeat::Item*>(p->getnext())) {
      switch (p->type) {
        case Item::TYPE_KEYBOARD:
        {
          Params_KeyboardEventCallBack* params = p->params.params_KeyboardEventCallBack;
          if (params) {
            if (queue_->size() == 1) {
              params->repeat = true;
            } else {
              params->repeat = false;
            }
            EventOutputQueue::FireKey::fire(*params);
          }
          break;
        }

        case Item::TYPE_CONSUMER:
        {
          Params_KeyboardSpecialEventCallback* params = p->params.params_KeyboardSpecialEventCallback;
          if (params) {
            if (queue_->size() == 1) {
              params->repeat = true;
            } else {
              params->repeat = false;
            }
            EventOutputQueue::FireConsumer::fire(*params);
          }
          isconsumer = true;
          break;
        }
      }
    }

    if (isconsumer) {
      timer_.setTimeoutMS(config.get_repeat_consumer_wait());
    } else {
      timer_.setTimeoutMS(config.get_repeat_wait());
    }
  }
}
