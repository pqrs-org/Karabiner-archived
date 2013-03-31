#ifndef COMMONDATA_HPP
#define COMMONDATA_HPP

#include "base.hpp"
#include "KeyCode.hpp"
#include "LastPressedPhysicalKey.hpp"
#include "bridge.h"
#include <string.h>

namespace org_pqrs_KeyRemap4MacBook {
  class ParamsUnion;

  class CommonData {
  public:
    static bool initialize(void);
    static void terminate(void);

    static void setcurrent_ts(const AbsoluteTime& ts) { current_ts_ = ts; }
    static const AbsoluteTime& getcurrent_ts(void) { return current_ts_; }

    static void setcurrent_keyboardType(KeyboardType keyboardType) { current_keyboardType_ = keyboardType; }
    static KeyboardType getcurrent_keyboardType(void) { return current_keyboardType_; }

    static void setcurrent_deviceIdentifier(const DeviceIdentifier& deviceIdentifier) {
      current_deviceIdentifier_ = deviceIdentifier;
    }
    static const DeviceIdentifier& getcurrent_deviceIdentifier(void) { return current_deviceIdentifier_; }

    static void setcurrent_workspacedata(const BridgeWorkSpaceData& newval) { current_workspacedata_ = newval; }
    static const BridgeWorkSpaceData& getcurrent_workspacedata(void) { return current_workspacedata_; }

    static void setcurrent_lastpressedphysicalkey(const ParamsUnion& newval) {
      current_lastpressedphysicalkey_.update(newval);
    }
    static const LastPressedPhysicalKey& getcurrent_lastpressedphysicalkey(void) {
      return current_lastpressedphysicalkey_;
    }

    static void clear_statusmessage(int index);
    static void append_statusmessage(int index, const char* message);
    static void send_notification_statusmessage(int index);
    static const char* get_statusmessage(int index);

    // for debug
    static void increase_alloccount(void);
    static void decrease_alloccount(void);

  private:
    static AbsoluteTime current_ts_;
    static KeyboardType current_keyboardType_;
    static DeviceIdentifier current_deviceIdentifier_;
    static BridgeWorkSpaceData current_workspacedata_;
    static LastPressedPhysicalKey current_lastpressedphysicalkey_;

    static char statusmessage_[BRIDGE_USERCLIENT_STATUS_MESSAGE__END__][BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN];

    static int alloccount_;
  };
}

#endif
