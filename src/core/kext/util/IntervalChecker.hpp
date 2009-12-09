#ifndef INTERVALCHECKER_HPP
#define INTERVALCHECKER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class IntervalChecker {
  public:
    void begin(void);
    bool checkThreshold(uint32_t millisec);

  private:
    clock_sec_t secs_;
    clock_usec_t microsecs_;
  };
}

#endif
