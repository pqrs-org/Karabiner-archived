#ifndef TIMERWRAPPER_HPP
#define TIMERWRAPPER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class TimerWrapper {
  public:
    TimerWrapper(void) { workLoop = NULL; timer = NULL; }
    void initialize(IOWorkLoop *_workLoop, OSObject *owner, IOTimerEventSource::Action func);
    void terminate(void);

    IOReturn setTimeoutMS(UInt32 ms);
    void cancelTimeout(void);

  private:
    IOWorkLoop *workLoop;
    IOTimerEventSource *timer;
  };
}

#endif
