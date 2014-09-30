#ifndef ORG_PQRS_TYPES_HPP
#define ORG_PQRS_TYPES_HPP

#include <IOKit/IOLib.h>
#include <IOKit/IOTimerEventSource.h>
#include "bridge.h"
#include "Vector.hpp"

namespace org_pqrs_Karabiner {
class AddDataType {
public:
  AddDataType(void) : value_(BRIDGE_DATATYPE_NONE) {}
  explicit AddDataType(unsigned int v) : value_(v) {}
  operator unsigned int(void) {
    return value_;
  }

private:
  unsigned int value_;
};

class AddValue {
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

class AddValueWithDataType {
public:
  AddValueWithDataType(void) {}
  AddValueWithDataType(AddDataType d, AddValue v) : dataType(d), value(v) {}
  AddDataType dataType;
  AddValue value;
};
DECLARE_VECTOR(AddValueWithDataType);
}

#endif
