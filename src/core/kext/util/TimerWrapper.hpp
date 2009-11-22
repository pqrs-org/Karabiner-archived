#ifndef TIMERWRAPPER_HPP
#define TIMERWRAPPER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class TimerWrapper {
  public:
    TimerWrapper(void) : workloop_(NULL), timer_(NULL) {}
    void initialize(IOWorkLoop* wl, OSObject* owner, IOTimerEventSource::Action func);
    void terminate(void);

    IOReturn setTimeoutMS(UInt32 ms);
    void cancelTimeout(void);

  private:
    IOWorkLoop* workloop_;
    IOTimerEventSource* timer_;
  };
}

#endif
