#include "KeyboardRepeat.hpp"
#include "ListHookedKeyboard.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  TimerWrapper KeyboardRepeat::timer_;
  Params_KeyboardEventCallBack KeyboardRepeat::params_;
  KeyRemap4MacBook_bridge::GetWorkspaceData::Reply KeyboardRepeat::workspacedata_;

  void
  KeyboardRepeat::initialize(IOWorkLoop& workloop)
  {
    params_.charCode = 0;
    params_.charSet = 0;
    params_.origCharCode = 0;
    params_.origCharSet = 0;
    params_.repeat = true;
    params_.refcon = NULL;

    timer_.initialize(&workloop, NULL, KeyboardRepeat::fire);
  }

  void
  KeyboardRepeat::terminate(void)
  {
    timer_.terminate();
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
      if (key == params_.key) {
        goto cancel;
      }

    } else if (eventType == EventType::DOWN) {
      if (key == KeyCode::VK_NONE) goto cancel;

      // set keyrepeat
      {
        HookedKeyboard* hk = ListHookedKeyboard::instance().get();
        if (! hk) goto cancel;

        params_.target = hk->getOrig_keyboardEventTarget();
        params_.eventType = eventType;
        params_.flags = flags;
        params_.key = key;
        params_.keyboardType = keyboardType;
        params_.sender = hk->get();

        timer_.setTimeoutMS(wait);
        if (config.debug_devel) {
          IOLog("KeyRemap4MacBook -Info- setRepeat_keyboard key:%d flags:0x%x\n", key.get(), flags.get());
        }
      }

    } else {
      goto cancel;
    }

    return;

  cancel:
    timer_.cancelTimeout();
  }

  void
  KeyboardRepeat::fire(OSObject* owner, IOTimerEventSource* sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    RemapUtil::fireKey(params_, workspacedata_);
    timer_.setTimeoutMS(config.get_repeat_wait());
  }
}
