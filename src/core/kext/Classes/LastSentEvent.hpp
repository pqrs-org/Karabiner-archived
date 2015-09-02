#ifndef LASTSENTEVENT_HPP
#define LASTSENTEVENT_HPP

#include "bridge.h"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_Karabiner {
class Params_Base;

class LastSentEvent final {
public:
  LastSentEvent(void) : datatype_(BRIDGE_DATATYPE_NONE),
                        value_(0),
                        eventtype_(EventType::NONE) {}

  void update(const Params_Base& newval);

  uint32_t get_datatype(void) const { return datatype_; }
  uint32_t get_value(void) const { return value_; }
  EventType get_eventtype(void) const { return eventtype_; }
  uint32_t get_milliseconds(void) const { return ic_.getmillisec(); }

private:
  uint32_t datatype_;
  uint32_t value_;
  EventType eventtype_;
  IntervalChecker ic_;
};
}

#endif
