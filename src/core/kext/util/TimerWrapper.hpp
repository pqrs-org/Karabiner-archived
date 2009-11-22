#ifndef TIMERWRAPPER_HPP
#define TIMERWRAPPER_HPP

#include "base.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class TimerWrapper {
  public:
    TimerWrapper(void) : workloop_(NULL), timer_(NULL), lock_(NULL) {}
    void initialize(IOWorkLoop* wl, OSObject* owner, IOTimerEventSource::Action func);
    void terminate(void);

    IOReturn setTimeoutMS(UInt32 ms);
    void cancelTimeout(void);

    IOLock* getlock(void) { return lock_; }

  private:
    IOWorkLoop* workloop_;
    IOTimerEventSource* timer_;
    IOLock* lock_;
  };
}

#endif
