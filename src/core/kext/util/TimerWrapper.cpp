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
    timer_ = IOTimerEventSource::timerEventSource(owner, func);

    if (workloop_->addEventSource(timer_) != kIOReturnSuccess) {
      IOLog("[KeyRemap4MacBook ERROR] TimerWrapper addEventSource failed\n");
      timer_->release();
      timer_ = NULL;
    }
  }

  void
  TimerWrapper::terminate(void)
  {
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

  IOReturn
  TimerWrapper::setTimeoutMS(UInt32 ms)
  {
    if (! timer_) {
      return kIOReturnNoResources;
    }
    return timer_->setTimeoutMS(ms);
  }

  void
  TimerWrapper::cancelTimeout(void)
  {
    if (timer_) {
      timer_->cancelTimeout();
    }
  }
}
