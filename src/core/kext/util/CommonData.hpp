#ifndef COMMONDATA_HPP
#define COMMONDATA_HPP

#include "base.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class CommonData {
  public:
    static void setcurrent_ts(const AbsoluteTime& ts) { current_ts_ = ts; }
    static const AbsoluteTime& getcurrent_ts(void) { return current_ts_; }

    static void setcurrent_keyboardType(const KeyboardType& keyboardType) { current_keyboardType_ = keyboardType; }
    static const KeyboardType& getcurrent_keyboardType(void) { return current_keyboardType_; }

  private:
    static AbsoluteTime current_ts_;
    static KeyboardType current_keyboardType_;
  };
}

#endif
