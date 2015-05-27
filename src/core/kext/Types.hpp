#ifndef ORG_PQRS_TYPES_HPP
#define ORG_PQRS_TYPES_HPP

#include <IOKit/IOLib.h>
#include <IOKit/IOTimerEventSource.h>
#include "bridge.h"
#include "Vector.hpp"

namespace org_pqrs_Karabiner {
class AddDataType final {
public:
  AddDataType(void) : value_(BRIDGE_DATATYPE_NONE) {}
  explicit AddDataType(unsigned int v) : value_(v) {}
  operator unsigned int(void) {
    return value_;
  }

private:
  unsigned int value_;
};

class AddValue final {
public:
  AddValue(void) : value_(0) {}
  explicit AddValue(unsigned int v) : value_(v) {}
  operator unsigned int(void) {
    return value_;
  }

private:
  unsigned int value_;
};
DECLARE_VECTOR(AddValue);

class AddValueWithDataType final {
public:
  AddValueWithDataType(void) {}
  AddValueWithDataType(AddDataType d, AddValue v) : dataType(d), value(v) {}
  AddDataType dataType;
  AddValue value;
};
DECLARE_VECTOR(AddValueWithDataType);
}

class AutogenId final {
public:
  explicit AutogenId(uint64_t v) : value_(v) {}
  operator uint64_t(void) {
    return value_;
  }
  bool operator>(AutogenId other) const { return value_ > other.value_; }
  bool operator>=(AutogenId other) const { return value_ >= other.value_; }
  bool operator<(AutogenId other) const { return value_ < other.value_; }
  bool operator<=(AutogenId other) const { return value_ <= other.value_; }

  static AutogenId maxValue(void) {
    //       1234567812345678
    return AutogenId(0xffffffffffffffff);
  }

private:
  uint64_t value_;
};

enum class PhysicalEventType {
  DOWN,
  UP,
  // We do not provide PhysicalEventType::MODIFY. Use DOWN or UP for modifier events.
};

#endif
