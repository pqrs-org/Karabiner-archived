#ifndef INTERVALCHECKER_HPP
#define INTERVALCHECKER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class IntervalChecker {
  public:
    void begin(void);
    uint32_t getmillisec(void) const;
    bool checkThreshold(uint32_t millisec);

  private:
    clock_sec_t secs_;
    clock_usec_t microsecs_;
  };
}

#endif
