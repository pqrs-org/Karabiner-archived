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

    static void setcurrent_keyboardType(KeyboardType keyboardType) { current_keyboardType_ = keyboardType; }
    static KeyboardType getcurrent_keyboardType(void) { return current_keyboardType_; }

    static void setcurrent_vendorIDproductID(DeviceVendorID vendorID, DeviceProductID productID) {
      current_deviceVendorID_ = vendorID;
      current_deviceProductID_ = productID;
    }
    static bool isEqualVendorIDProductID(DeviceVendorID vendorID, DeviceProductID productID) {
      if (current_deviceVendorID_  != vendorID) return false;
      if (current_deviceProductID_ != productID) return false;
      return true;
    }

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
    static DeviceVendorID current_deviceVendorID_;
    static DeviceProductID current_deviceProductID_;
  };
}

#endif
