#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  TimerWrapper::initialize(IOWorkLoop* wl, OSObject* owner, IOTimerEventSource::Action func)
  {
    if (timer_) terminate();

    if (! wl) return;

    workloop_ = wl;
    if (owner == NULL) {
      owner = reinterpret_cast<OSObject*>(this);
    }
    action_ = func;
    action_owner_ = owner;
    timer_ = IOTimerEventSource::timerEventSource(reinterpret_cast<OSObject*>(this), callback_);

    if (workloop_->addEventSource(timer_) != kIOReturnSuccess) {
      IOLog("[KeyRemap4MacBook ERROR] TimerWrapper addEventSource failed\n");
      timer_->release();
      timer_ = NULL;
    }

    lock_ = IOLockWrapper::alloc();
    lock_active_ = IOLockWrapper::alloc();
  }

  void
  TimerWrapper::terminate(void)
  {
    {
      IOLockWrapper::ScopedLock lk(lock_);

      if (timer_) {
        timer_->cancelTimeout();
        if (workloop_) {
          workloop_->removeEventSource(timer_);
        }
        timer_->release();
        timer_ = NULL;
      }
      workloop_ = NULL;
    }

    IOLockWrapper::free(lock_);
    IOLockWrapper::free(lock_active_);
  }

  IOReturn
  TimerWrapper::setTimeoutMS(UInt32 ms, bool overwrite)
  {
    IOLockWrapper::ScopedLock lk(lock_active_);

    if (! timer_) {
      return kIOReturnNoResources;
    }
    if (! overwrite && active_) {
      return kIOReturnSuccess;
    }

    active_ = true;
    return timer_->setTimeoutMS(ms);
  }

  void
  TimerWrapper::cancelTimeout(void)
  {
    if (timer_) {
      timer_->cancelTimeout();
    }
  }

  void
  TimerWrapper::callback_(OSObject* owner, IOTimerEventSource* sender)
  {
    TimerWrapper* self = reinterpret_cast<TimerWrapper*>(owner);
    if (! self) return;

    {
      IOLockWrapper::ScopedLock lk(self->lock_active_);
      self->active_ = false;
    }
    self->action_(self->action_owner_, sender);
  }
}
