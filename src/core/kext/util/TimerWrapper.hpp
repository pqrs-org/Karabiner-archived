#ifndef TIMERWRAPPER_HPP
#define TIMERWRAPPER_HPP

#include "base.hpp"
#include "IOLockWrapper.hpp"

class org_pqrs_KeyRemap4MacBook_TimerWrapperObject : public OSObject {
  OSDeclareDefaultStructors(org_pqrs_KeyRemap4MacBook_TimerWrapperObject);

public:
  static org_pqrs_KeyRemap4MacBook_TimerWrapperObject* timerEventSource(OSObject* owner, IOTimerEventSource::Action action);
  bool init(OSObject* owner, IOTimerEventSource::Action action);
  void free(void);

  OSObject* getowner(void) const { return owner_; }
  IOLock* getlock(void) const { return lock_; }
  IOTimerEventSource::Action getaction(void) const { return action_; }

  bool isActive(void) const { return active_; }
  void setActive(bool newval) { active_ = newval; }

private:
  OSObject* owner_;
  IOTimerEventSource::Action action_;
  IOLock* lock_;
  bool active_;
};

namespace org_pqrs_KeyRemap4MacBook {
  class TimerWrapper {
  public:
    TimerWrapper(void) : workloop_(NULL), object_(NULL), timer_(NULL), lock_(NULL) {}
    void initialize(IOWorkLoop* wl, OSObject* owner, IOTimerEventSource::Action func);
    void terminate(void);

    // if overwrite == false, we don't set timeout if timer is already active.
    IOReturn setTimeoutMS(UInt32 ms, bool overwrite = true);
    void cancelTimeout(void);

    IOLock* getlock(void) { return lock_; }

  private:
    static void callback_(OSObject* owner, IOTimerEventSource* sender);

    IOWorkLoop* workloop_;
    org_pqrs_KeyRemap4MacBook_TimerWrapperObject* object_;
    IOTimerEventSource* timer_;
    IOLock* lock_;
  };
}

#endif
