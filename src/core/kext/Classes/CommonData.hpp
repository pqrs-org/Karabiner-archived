#ifndef COMMONDATA_HPP
#define COMMONDATA_HPP

#include "KeyCode.hpp"
#include "IOLogWrapper.hpp"
#include "LastPressedPhysicalKey.hpp"
#include "LastReleasedPhysicalKey.hpp"
#include "LastSentEvent.hpp"
#include "bridge.h"
#include <string.h>

namespace org_pqrs_Karabiner {
class Params_Base;

class CommonData final {
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

  static void setcurrent_workspaceIds(const uint32_t* ids, size_t count);
  static const Vector_WorkspaceAppId& getcurrent_workspaceAppIds(void) { return current_workspaceAppIds_; }
  static const Vector_WorkspaceWindowNameId& getcurrent_workspaceWindowNameIds(void) { return current_workspaceWindowNameIds_; }
  static const WorkspaceUIElementRoleId getcurrent_workspaceUIElementRoleId(void) { return current_workspaceUIElementRoleId_; }
  static const Vector_WorkspaceInputSourceId& getcurrent_workspaceInputSourceIds(void) { return current_workspaceInputSourceIds_; }

  static void setcurrent_lastpressedphysicalkey(const Params_Base& newval) {
    current_lastpressedphysicalkey_.update(newval);
  }
  static const LastPressedPhysicalKey& getcurrent_lastpressedphysicalkey(void) {
    return current_lastpressedphysicalkey_;
  }
  static void setcurrent_lastreleasedphysicalkey(const Params_Base& newval) {
    current_lastreleasedphysicalkey_.update(newval);
  }
  static const LastReleasedPhysicalKey& getcurrent_lastreleasedphysicalkey(void) {
    return current_lastreleasedphysicalkey_;
  }
  static void setcurrent_lastsentevent(const Params_Base& newval) {
    current_lastsentevent_.update(newval);
    IOLOG_DEVEL("LastSentEvent: %s:0x%04x, %s\n",
                (current_lastsentevent_.get_datatype() == BRIDGE_DATATYPE_KEYCODE
                     ? "KeyCode"
                     : (current_lastsentevent_.get_datatype() == BRIDGE_DATATYPE_CONSUMERKEYCODE
                            ? "ConsumerKeyCode"
                            : (current_lastsentevent_.get_datatype() == BRIDGE_DATATYPE_POINTINGBUTTON
                                   ? "PointingButton"
                                   : (current_lastsentevent_.get_datatype() == BRIDGE_DATATYPE_SCROLLWHEEL
                                          ? "ScrollWheel"
                                          : "Unknown")))),
                current_lastsentevent_.get_value(),
                (current_lastsentevent_.get_eventtype() == EventType::NONE
                     ? "EventType::NONE"
                     : (current_lastsentevent_.get_eventtype() == EventType::DOWN
                            ? "EventType::DOWN"
                            : (current_lastsentevent_.get_eventtype() == EventType::UP
                                   ? "EventType::UP"
                                   : (current_lastsentevent_.get_eventtype() == EventType::MODIFY
                                          ? "EventType::MODIFY"
                                          : "Unknown")))));
  }
  static const LastSentEvent& getcurrent_lastsentevent(void) {
    return current_lastsentevent_;
  }

  static void clear_statusmessage(int index);
  static void append_statusmessage(int index, const char* message);
  static void send_notification_statusmessage(int index);
  static const char* get_statusmessage(int index);

private:
  static AbsoluteTime current_ts_;
  static KeyboardType current_keyboardType_;
  static DeviceIdentifier current_deviceIdentifier_;
  static Vector_WorkspaceAppId current_workspaceAppIds_;
  static Vector_WorkspaceInputSourceId current_workspaceInputSourceIds_;
  static WorkspaceUIElementRoleId current_workspaceUIElementRoleId_;
  static Vector_WorkspaceWindowNameId current_workspaceWindowNameIds_;
  static LastPressedPhysicalKey current_lastpressedphysicalkey_;
  static LastReleasedPhysicalKey current_lastreleasedphysicalkey_;
  static LastSentEvent current_lastsentevent_;

  static char statusmessage_[BRIDGE_USERCLIENT_STATUS_MESSAGE__END__][BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN];
};
}

#endif
