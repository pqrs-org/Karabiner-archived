#ifndef INTERVALCHECKER_HPP
#define INTERVALCHECKER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class IntervalChecker {
  public:
    void begin(void);
    bool checkThreshold(uint32_t millisec);

  private:
    uint32_t secs;
    uint32_t microsecs;
  };
}

#endif
