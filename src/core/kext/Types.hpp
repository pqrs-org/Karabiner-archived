#ifndef ORG_PQRS_TYPES_HPP
#define ORG_PQRS_TYPES_HPP

#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
#include <IOKit/IOTimerEventSource.h>
END_IOKIT_INCLUDE;

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
  operator uint64_t(void) const {
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

class SerialNumber {
public:
  // Set initial value to 1. (Use 0 as invalid value.)
  SerialNumber(void) : value_(1) {}
  explicit SerialNumber(uint64_t v) : value_(v) {}
  // Do not provide virtual destructor.

  uint64_t get(void) const { return value_; }
  void set(const SerialNumber& v) { value_ = v.value_; }

  SerialNumber& operator++(void) {
    ++value_;
    return *this;
  }
  SerialNumber& operator--(void) {
    --value_;
    return *this;
  }
  bool operator==(SerialNumber other) const { return value_ == other.get(); }
  bool operator!=(SerialNumber other) const { return !(*this == other); }
  bool operator>(SerialNumber other) const { return value_ > other.value_; }
  bool operator>=(SerialNumber other) const { return value_ >= other.value_; }
  bool operator<(SerialNumber other) const { return value_ < other.value_; }
  bool operator<=(SerialNumber other) const { return value_ <= other.value_; }
  SerialNumber operator+(int v) { return SerialNumber(value_ + v); }
  SerialNumber operator-(int v) { return SerialNumber(value_ - v); }

  void reset(void) { value_ = 1; }

private:
  uint64_t value_;
};

// ========================================
#define DECLARE_WORKSPACE_CLASS(CLASSNAME)                                   \
  class CLASSNAME final {                                                    \
  public:                                                                    \
    CLASSNAME(void) : value_(0) {}                                           \
    explicit CLASSNAME(uint32_t v) : value_(v) {}                            \
    uint32_t get(void) const { return value_; }                              \
    bool operator==(CLASSNAME other) const { return value_ == other.get(); } \
    bool operator!=(CLASSNAME other) const { return !(*this == other); }     \
                                                                             \
  private:                                                                   \
    uint32_t value_;                                                         \
  };

DECLARE_WORKSPACE_CLASS(WorkspaceAppId);
DECLARE_VECTOR_WITH_HELPER(WorkspaceAppId);

DECLARE_WORKSPACE_CLASS(WorkspaceInputSourceId);
DECLARE_VECTOR_WITH_HELPER(WorkspaceInputSourceId);

DECLARE_WORKSPACE_CLASS(WorkspaceUIElementRoleId);
DECLARE_VECTOR_WITH_HELPER(WorkspaceUIElementRoleId);

DECLARE_WORKSPACE_CLASS(WorkspaceWindowNameId);
DECLARE_VECTOR_WITH_HELPER(WorkspaceWindowNameId);

#endif
