#ifndef ORG_PQRS_TYPES_HPP
#define ORG_PQRS_TYPES_HPP

#include <IOKit/IOLib.h>
#include <IOKit/IOTimerEventSource.h>

namespace org_pqrs_KeyRemap4MacBook {
  class AddDataType {
  public:
    explicit AddDataType(unsigned int v = 0) : value_(v) {}
    operator unsigned int(void) {
      return value_;
    }

  private:
    unsigned int value_;
  };

  class AddValue {
  public:
    explicit AddValue(unsigned int v = 0) : value_(v) {}
    operator unsigned int(void) {
      return value_;
    }

  private:
    unsigned int value_;
  };
}

#endif
