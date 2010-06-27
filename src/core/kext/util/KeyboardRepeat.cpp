#include "CommonData.hpp"
#include "EventOutput.hpp"
#include "IOLockWrapper.hpp"
#include "KeyboardRepeat.hpp"
#include "ListHookedKeyboard.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  TimerWrapper KeyboardRepeat::timer_;
  KeyboardRepeat::Item KeyboardRepeat::item_[KeyboardRepeat::MAXNUM];

  void
  KeyboardRepeat::initialize(IOWorkLoop& workloop)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].params = Params_KeyboardEventCallBack::alloc(EventType(0), Flags(0), KeyCode(0), KeyboardType(0), true);
      item_[i].params_consumer = Params_KeyboardSpecialEventCallback::alloc(EventType(0), Flags(0), ConsumerKeyCode(0), true);
    }

    timer_.initialize(&workloop, NULL, KeyboardRepeat::fire);
  }

  void
  KeyboardRepeat::terminate(void)
  {
    timer_.terminate();

    for (int i = 0; i < MAXNUM; ++i) {
      {
        Params_KeyboardEventCallBack* p = item_[i].params;
        if (p) delete p;
      }
      {
        Params_KeyboardSpecialEventCallback* p = item_[i].params_consumer;
        if (p) delete p;
      }
    }
  }

  void
  KeyboardRepeat::cancel_nolock(void)
  {
    timer_.cancelTimeout();
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].active = false;
    }
  }

  void
  KeyboardRepeat::cancel(void)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    cancel_nolock();
  }

  void
  KeyboardRepeat::primitive_add_nolock(EventType eventType,
                                       Flags flags,
                                       KeyCode key,
                                       KeyboardType keyboardType)
  {
    if (key == KeyCode::VK_NONE) return;

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].active) {
        Params_KeyboardEventCallBack* p = item_[i].params;
        if (! p) return;

        p->eventType = eventType;
        p->flags = flags;
        p->key = key;
        p->keyboardType = keyboardType;
        item_[i].active = true;
        item_[i].type = Item::TYPE_KEYBOARD;

        return;
      }
    }
    IOLog("KeyRemap4MacBook --Warning-- KeyboardRepeat::Item was filled up. Expand MAXNUM.\n");
  }

  void
  KeyboardRepeat::primitive_add_nolock(EventType eventType,
                                       Flags flags,
                                       ConsumerKeyCode key)
  {
    if (key == ConsumerKeyCode::VK_NONE) return;

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].active) {
        Params_KeyboardSpecialEventCallback* p = item_[i].params_consumer;
        if (! p) return;

        p->eventType = eventType;
        p->flags = flags;
        p->key = key;
        p->flavor = key.get();
        item_[i].active = true;
        item_[i].type = Item::TYPE_CONSUMER;

        return;
      }
    }
    IOLog("KeyRemap4MacBook --Warning-- KeyboardRepeat::Item was filled up. Expand MAXNUM.\n");
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

  int
  KeyboardRepeat::getActiveItemNum(void)
  {
    int num = 0;
    for (int i = 0; i < MAXNUM; ++i) {
      num += item_[i].active;
    }
    return num;
  }

  void
  KeyboardRepeat::set(EventType eventType,
                      Flags flags,
                      KeyCode key,
                      KeyboardType keyboardType,
                      int wait)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (eventType == EventType::MODIFY) {
      goto cancel;

    } else if (eventType == EventType::UP) {
      // We stop key repeat only when the repeating key is up.
      if (getActiveItemNum() == 1 &&
          key == (item_[0].params)->key) {
        goto cancel;
      }

    } else if (eventType == EventType::DOWN) {
      if (key == KeyCode::VK_NONE) goto cancel;

      cancel_nolock();

      primitive_add_nolock(eventType, flags, key, keyboardType);
      primitive_start_nolock(wait);

      if (config.debug_devel) {
        IOLog("KeyRemap4MacBook -Info- setRepeat_keyboard key:%d flags:0x%x\n", key.get(), flags.get());
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
        IOLog("KeyRemap4MacBook -Info- setRepeat_keyboard consumer key:%d flags:0x%x\n", key.get(), flags.get());
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

    bool isconsumer = false;

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].active) {
        break;

      } else {
        switch (item_[i].type) {
          case Item::TYPE_KEYBOARD:
          {
            Params_KeyboardEventCallBack* p = item_[i].params;
            if (p) {
              EventOutput::FireKey::fire(*p);
            }
            break;
          }

          case Item::TYPE_CONSUMER:
          {
            Params_KeyboardSpecialEventCallback* p = item_[i].params_consumer;
            if (p) {
              EventOutput::FireConsumer::fire(*p);
            }
            isconsumer = true;
            break;
          }
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
