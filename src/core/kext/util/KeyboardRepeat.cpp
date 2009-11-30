#include "KeyboardRepeat.hpp"
#include "ListHookedKeyboard.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  TimerWrapper KeyboardRepeat::timer_;
  Params_KeyboardEventCallBack KeyboardRepeat::params_;

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
  KeyboardRepeat::set(KeyEvent::KeyEvent eventType, unsigned int flags, KeyCode::KeyCode key, KeyboardType::KeyboardType keyboardType, int wait)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    switch (eventType) {
      case KeyEvent::MODIFY:
        goto cancel;

      case KeyEvent::UP:
        if (static_cast<unsigned int>(key) == params_.key) goto cancel;
        break;

      case KeyEvent::DOWN:
        if (key == KeyCode::NONE) goto cancel;

        {
          HookedKeyboard* hk = ListHookedKeyboard::instance().get();
          if (! hk) goto cancel;

          params_.target = hk->getOrig_keyboardEventTarget();
          params_.eventType = eventType;
          params_.flags = flags;
          params_.key = key;
          params_.keyboardType = keyboardType;
          params_.sender = hk->get();

          {
            IOReturn result = timer_.setTimeoutMS(wait);
            if (result != kIOReturnSuccess) {
              IOLog("[KeyRemap4MacBook ERROR] setTimeoutMS failed\n");
            }
          }
        }

        if (config.debug_devel) {
          IOLog("KeyRemap4MacBook -Info- setRepeat_keyboard key:%d flags:0x%x\n", key, flags);
        }
        break;

      default:
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

    HookedKeyboard* p = ListHookedKeyboard::instance().get();
    if (! p) return;

    KeyboardEventCallback callback = p->getOrig_keyboardEventAction();
    if (! callback) {
      IOLog("KeyRemap4MacBook -Info- doRepeat_keyboard: callback == NULL (== don't remap xxx is enabled). \n");
      return;
    }

    RemapUtil::fireKey(callback, params_);
    timer_.setTimeoutMS(config.get_repeat_wait());
  }
}
