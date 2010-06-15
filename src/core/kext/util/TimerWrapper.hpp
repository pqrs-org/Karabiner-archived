#ifndef TIMERWRAPPER_HPP
#define TIMERWRAPPER_HPP

#include "base.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class TimerWrapper {
  public:
    TimerWrapper(void) : workloop_(NULL), timer_(NULL), lock_(NULL), lock_active_(NULL), active_(false), action_(NULL), action_owner_(NULL) {}
    void initialize(IOWorkLoop* wl, OSObject* owner, IOTimerEventSource::Action func);
    void terminate(void);

    // if overwrite == false, we don't set timeout if timer is already active.
    IOReturn setTimeoutMS(UInt32 ms, bool overwrite = true);
    void cancelTimeout(void);

    IOLock* getlock(void) { return lock_; }

  private:
    static void callback_(OSObject* owner, IOTimerEventSource* sender);

    IOWorkLoop* workloop_;
    IOTimerEventSource* timer_;
    IOLock* lock_;

    IOLock* lock_active_;
    bool active_;

    IOTimerEventSource::Action action_;
    OSObject* action_owner_;
  };
}

#endif
