#ifndef LASTRELEASEDPHYSICALKEY_HPP
#define LASTRELEASEDPHYSICALKEY_HPP

#include "IntervalChecker.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
class Params_Base;

class LastReleasedPhysicalKey final {
public:
  LastReleasedPhysicalKey(void) : datatype_(BRIDGE_DATATYPE_NONE),
                                  value_(0) {}

  void update(const Params_Base& newval);

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
