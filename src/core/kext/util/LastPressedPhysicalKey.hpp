#ifndef LASTPRESSEDPHYSICALKEY_HPP
#define LASTPRESSEDPHYSICALKEY_HPP

#include "bridge.h"
#include "IntervalChecker.hpp"

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
    uint32_t get_milliseconds(void) const { return ic_.getmillisec(); }

  private:
    uint32_t datatype_;
    uint32_t value_;
    IntervalChecker ic_;
  };
}

#endif
