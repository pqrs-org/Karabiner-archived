#include "IntervalChecker.hpp"
#include <sys/systm.h>

namespace org_pqrs_KeyRemap4MacBook {
  void
  IntervalChecker::begin(void)
  {
    clock_get_system_microtime(&secs_, &microsecs_);
  }

  uint32_t
  IntervalChecker::getmillisec(void) const
  {
    clock_sec_t s;
    clock_usec_t m;
    clock_get_system_microtime(&s, &m);

    uint32_t interval = static_cast<int>(s - secs_) * 1000 + static_cast<int>(m - microsecs_) / 1000;
    return interval;
  }

  bool
  IntervalChecker::checkThreshold(uint32_t millisec)
  {
    return getmillisec() >= millisec;
  }
}
