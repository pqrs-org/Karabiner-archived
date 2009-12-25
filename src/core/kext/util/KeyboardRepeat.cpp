#include "CommonData.hpp"
#include "KeyboardRepeat.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedKeyboard.hpp"
#include "RemapUtil.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  TimerWrapper KeyboardRepeat::timer_;
  KeyboardRepeat::Item KeyboardRepeat::item_[KeyboardRepeat::MAXNUM];
  KeyRemap4MacBook_bridge::GetWorkspaceData::Reply KeyboardRepeat::workspacedata_;

  void
  KeyboardRepeat::initialize(IOWorkLoop& workloop)
  {
    timer_.initialize(&workloop, NULL, KeyboardRepeat::fire);
  }

  void
  KeyboardRepeat::terminate(void)
  {
    timer_.terminate();
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
  KeyboardRepeat::primitive_add_nolock(const EventType& eventType,
                                       const Flags& flags,
                                       const KeyCode& key,
                                       const KeyboardType& keyboardType)
  {
    if (key == KeyCode::VK_NONE) return;

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item_[i].active) {
        item_[i].params.eventType = eventType;
        item_[i].params.flags = flags;
        item_[i].params.key = key;
        item_[i].params.keyboardType = keyboardType;
        item_[i].active = true;

        return;
      }
    }
    IOLog("KeyRemap4MacBook --Warning-- KeyboardRepeat::Item was filled up. Expand MAXNUM.\n");
  }

  void
  KeyboardRepeat::primitive_add(const EventType& eventType,
                                const Flags& flags,
                                const KeyCode& key,
                                const KeyboardType& keyboardType)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    primitive_add_nolock(eventType, flags, key, keyboardType);
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
  KeyboardRepeat::set(const EventType& eventType,
                      const Flags& flags,
                      const KeyCode& key,
                      const KeyboardType& keyboardType,
                      int wait)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (eventType == EventType::MODIFY) {
      goto cancel;

    } else if (eventType == EventType::UP) {
      // We stop key repeat only when the repeating key is up.
      if (getActiveItemNum() == 1 &&
          key == item_[0].params.key) {
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
  KeyboardRepeat::fire(OSObject* owner, IOTimerEventSource* sender)
  {
    if (! CommonData::eventLock) return;
    IOLockWrapper::ScopedLock eventLock(CommonData::eventLock);

    // ------------------------------------------------------------
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    for (int i = 0; i < MAXNUM; ++i) {
      if (item_[i].active) {
        RemapUtil::fireKey(item_[i].params, workspacedata_);
      }
    }

    timer_.setTimeoutMS(config.get_repeat_wait());
  }
}
