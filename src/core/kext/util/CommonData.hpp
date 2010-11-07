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

    static void setcurrent_vendorProduct(DeviceVendor vendor, DeviceProduct product) {
      current_deviceVendor_ = vendor;
      current_deviceProduct_ = product;
    }
    static bool isEqualVendorProduct(DeviceVendor vendor, DeviceProduct product) {
      if (current_deviceVendor_  != vendor) return false;
      if (current_deviceProduct_ != product) return false;
      return true;
    }

    static void setcurrent_workspacedata(void);
    static const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& getcurrent_workspacedata(void) { return current_workspacedata_; }

    // for debug
    static void increase_alloccount(void);
    static void decrease_alloccount(void);

    // We need to use EventLock to guard FlagStatus, NumHeldDownKeys, or other global variables.
    // (TimerWrapper use this lock to serialize timerevents, so we must not lock in timer callback.)
    //
    // *** We need to lock EventLock before other locks (list lock, etc.) to avoid the deadlock . ***
    static IOLock* getEventLock(void) { return event_lock_; }

  private:
    static AbsoluteTime current_ts_;
    static KeyboardType current_keyboardType_;
    static DeviceVendor current_deviceVendor_;
    static DeviceProduct current_deviceProduct_;
    static KeyRemap4MacBook_bridge::GetWorkspaceData::Reply current_workspacedata_;

    static int alloccount_;
    static IOLock* alloccount_lock_;

    static IOLock* event_lock_;
  };
}

#endif
