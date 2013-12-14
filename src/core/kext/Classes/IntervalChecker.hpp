#ifndef INTERVALCHECKER_HPP
#define INTERVALCHECKER_HPP

#include "Types.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class IntervalChecker {
  public:
    void begin(void);
    uint32_t getmillisec(void) const;

  private:
    clock_sec_t secs_;
    clock_usec_t microsecs_;
  };
}

#endif
