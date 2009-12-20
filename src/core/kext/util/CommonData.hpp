#ifndef COMMONDATA_HPP
#define COMMONDATA_HPP

#include "base.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class CommonData {
  public:
    static bool initialize(void);
    static void terminate(void);

    static void setcurrent_ts(const AbsoluteTime& ts) { current_ts_ = ts; }
    static const AbsoluteTime& getcurrent_ts(void) { return current_ts_; }

    static void setcurrent_keyboardType(const KeyboardType& keyboardType) { current_keyboardType_ = keyboardType; }
    static const KeyboardType& getcurrent_keyboardType(void) { return current_keyboardType_; }

    // We get the lock to save internal data (ex. FlagStatus) at
    //   - KeyboardEventCallback
    //   - KeyboardRepeat
    //   - KeyboardSpecialEventCallback
    //   - RelativePointerEventCallback
    //   - ScrollWheelEventCallback.
    static IOLock* eventLock;

  private:
    static AbsoluteTime current_ts_;
    static KeyboardType current_keyboardType_;
  };
}

#endif
