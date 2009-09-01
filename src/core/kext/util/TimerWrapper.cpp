#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  TimerWrapper::initialize(IOWorkLoop *_workLoop, OSObject *owner, IOTimerEventSource::Action func)
  {
    if (timer) terminate();

    if (! _workLoop) return;

    workLoop = _workLoop;
    if (owner == NULL) {
      owner = reinterpret_cast<OSObject *>(this);
    }
    timer = IOTimerEventSource::timerEventSource(owner, func);

    if (workLoop->addEventSource(timer) != kIOReturnSuccess) {
      IOLog("[KeyRemap4MacBook ERROR] TimerWrapper addEventSource failed\n");
      timer->release();
      timer = NULL;
    }
  }

  void
  TimerWrapper::terminate(void)
  {
    if (timer) {
      timer->cancelTimeout();
      if (workLoop) {
        workLoop->removeEventSource(timer);
      }
      timer->release();
      timer = NULL;
    }
    workLoop = NULL;
  }

  IOReturn
  TimerWrapper::setTimeoutMS(UInt32 ms)
  {
    if (! timer) {
      return kIOReturnNoResources;
    }
    return timer->setTimeoutMS(ms);
  }

  void
  TimerWrapper::cancelTimeout(void)
  {
    if (timer) {
      timer->cancelTimeout();
    }
  }
}
