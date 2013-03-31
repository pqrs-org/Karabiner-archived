#ifndef LASTPRESSEDPHYSICALKEY_HPP
#define LASTPRESSEDPHYSICALKEY_HPP

#include "base.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  class ParamsUnion;

  class LastPressedPhysicalKey {
  public:
    LastPressedPhysicalKey(void) :
      datatype_(BRIDGE_DATATYPE_NONE),
      value_(0)
    {}

    void update(const ParamsUnion& newval);

    uint32_t get_datatype(void) const { return datatype_; }
    uint32_t get_value(void) const { return value_; }

  private:
    void update_time(void);

    uint32_t datatype_;
    uint32_t value_;
    clock_sec_t secs_;
    clock_usec_t microsecs_;
  };
}

#endif
