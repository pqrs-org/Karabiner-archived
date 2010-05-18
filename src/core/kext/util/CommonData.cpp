#include "CommonData.hpp"
#include "IOLockWrapper.hpp"
#include "Config.hpp"
#include "Client.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  AbsoluteTime CommonData::current_ts_;
  KeyboardType CommonData::current_keyboardType_;
  DeviceVendorID CommonData::current_deviceVendorID_;
  DeviceProductID CommonData::current_deviceProductID_;
  KeyRemap4MacBook_bridge::GetWorkspaceData::Reply CommonData::current_workspacedata_;
  IOLock* CommonData::eventLock = NULL;

  bool
  CommonData::initialize(void)
  {
    eventLock = IOLockWrapper::alloc();
    if (! eventLock) return false;

    return true;
  }

  void
  CommonData::terminate(void)
  {
    IOLockWrapper::free(eventLock);
  }

  void
  CommonData::setcurrent_workspacedata(void)
  {
    // ------------------------------------------------------------
    // When we press the functional key (ex. F2) with the keyboard of the third vendor,
    // KeyRemap4MacBook_client::sendmsg returns EIO.
    //
    // We use the previous value when the error occurred.
    static KeyRemap4MacBook_bridge::GetWorkspaceData::Reply last = {
      KeyRemap4MacBook_bridge::GetWorkspaceData::UNKNOWN,
      KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_ROMAN,
      KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN,
    };

    int error = KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_GET_WORKSPACE_DATA, NULL, 0, &current_workspacedata_, sizeof(current_workspacedata_));
    if (config.debug_devel) {
      printf("KeyRemap4MacBook -Info- GetWorkspaceData: %d,%d,%d (error: %d)\n",
             current_workspacedata_.type,
             current_workspacedata_.inputmode,
             current_workspacedata_.inputmodedetail,
             error);
    }
    if (error == 0) {
      last = current_workspacedata_;
    } else {
      // use last info.
      current_workspacedata_ = last;
    }
  }
}
