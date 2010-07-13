#ifndef COMMONDATA_HPP
#define COMMONDATA_HPP

#include "base.hpp"
#include "KeyCode.hpp"
#include "bridge.hpp"

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

    static void setcurrent_workspacedata(void);
    static const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& getcurrent_workspacedata(void) { return current_workspacedata_; }

    // for debug
    static void increase_alloccount(void);
    static void decrease_alloccount(void);

    // We need to use EventLock to guard FlagStatus, NumHeldDownKeys, or other global variables.
    // *** We need to lock EventLock before other locks (timer lock, list lock, etc.) to avoid the deadlock . ***
    static IOLock* getEventLock(void) { return event_lock_; }

  private:
    static AbsoluteTime current_ts_;
    static KeyboardType current_keyboardType_;
    static DeviceVendorID current_deviceVendorID_;
    static DeviceProductID current_deviceProductID_;
    static KeyRemap4MacBook_bridge::GetWorkspaceData::Reply current_workspacedata_;

    static int alloccount_;
    static IOLock* alloccount_lock_;

    static IOLock* event_lock_;
  };
}

#endif
