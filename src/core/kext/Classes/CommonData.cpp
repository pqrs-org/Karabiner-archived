#include "CommonData.hpp"
#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "UserClient_kext.hpp"

namespace org_pqrs_Karabiner {
AbsoluteTime CommonData::current_ts_;
KeyboardType CommonData::current_keyboardType_;
DeviceIdentifier CommonData::current_deviceIdentifier_;
Vector_WorkspaceAppId CommonData::current_workspaceAppIds_;
Vector_WorkspaceInputSourceId CommonData::current_workspaceInputSourceIds_;
WorkspaceUIElementRoleId CommonData::current_workspaceUIElementRoleId_;
Vector_WorkspaceWindowNameId CommonData::current_workspaceWindowNameIds_;

LastPressedPhysicalKey CommonData::current_lastpressedphysicalkey_;
LastReleasedPhysicalKey CommonData::current_lastreleasedphysicalkey_;
LastSentEvent CommonData::current_lastsentevent_;
char CommonData::statusmessage_[BRIDGE_USERCLIENT_STATUS_MESSAGE__END__][BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN];

bool CommonData::initialize(void) {
  for (int i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
    statusmessage_[i][0] = '\0';
  }

  return true;
}

void CommonData::terminate(void) {}

void CommonData::setcurrent_workspaceIds(const uint32_t* ids, size_t count) {
  current_workspaceAppIds_.clear();
  current_workspaceWindowNameIds_.clear();
  current_workspaceUIElementRoleId_ = WorkspaceUIElementRoleId(0);
  current_workspaceInputSourceIds_.clear();

  for (int i = 0; i < static_cast<int>(count) - 1; i += 2) {
    uint32_t type = ids[i];
    uint32_t value = ids[i + 1];

    switch (type) {
    case BRIDGE_WORKSPACETYPE_NONE:
      break;

    case BRIDGE_WORKSPACETYPE_APP_ID:
      current_workspaceAppIds_.push_back(WorkspaceAppId(value));
      break;

    case BRIDGE_WORKSPACETYPE_INPUT_SOURCE_ID:
      current_workspaceInputSourceIds_.push_back(WorkspaceInputSourceId(value));
      break;

    case BRIDGE_WORKSPACETYPE_UI_ELEMENT_ROLE_ID:
      current_workspaceUIElementRoleId_ = WorkspaceUIElementRoleId(value);
      break;

    case BRIDGE_WORKSPACETYPE_WINDOW_NAME_ID:
      current_workspaceWindowNameIds_.push_back(WorkspaceWindowNameId(value));
      break;

    default:
      IOLOG_WARN("CommonData::setcurrent_workspaceIds invalid type: %d", type);
      break;
    }
  }
}

void CommonData::clear_statusmessage(int index) {
  if (index <= BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE) return;
  if (index >= BRIDGE_USERCLIENT_STATUS_MESSAGE__END__) return;

  statusmessage_[index][0] = '\0';
}

void CommonData::append_statusmessage(int index, const char* message) {
  if (index <= BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE) return;
  if (index >= BRIDGE_USERCLIENT_STATUS_MESSAGE__END__) return;

  strlcat(statusmessage_[index], message, sizeof(statusmessage_[index]));
}

void CommonData::send_notification_statusmessage(int index) {
  if (index <= BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE) return;
  if (index >= BRIDGE_USERCLIENT_STATUS_MESSAGE__END__) return;

  org_pqrs_driver_Karabiner_UserClient_kext::send_notification_to_userspace(BRIDGE_USERCLIENT_NOTIFICATION_TYPE_STATUS_MESSAGE_UPDATED, index);
}

const char* CommonData::get_statusmessage(int index) {
  if (index <= BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE) return nullptr;
  if (index >= BRIDGE_USERCLIENT_STATUS_MESSAGE__END__) return nullptr;

  return statusmessage_[index];
}
}
