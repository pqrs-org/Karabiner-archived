#include "CommonData.hpp"
#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "UserClient_kext.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  AbsoluteTime CommonData::current_ts_;
  KeyboardType CommonData::current_keyboardType_;
  DeviceIdentifier CommonData::current_deviceIdentifier_;
  BridgeWorkSpaceData CommonData::current_workspacedata_;
  LastPressedPhysicalKey CommonData::current_lastpressedphysicalkey_;
  char CommonData::statusmessage_[BRIDGE_USERCLIENT_STATUS_MESSAGE__END__][BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN];

  int CommonData::alloccount_;

  bool
  CommonData::initialize(void)
  {
    for (int i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
      statusmessage_[i][0] = '\0';
    }

    return true;
  }

  void
  CommonData::terminate(void)
  {}

  void
  CommonData::clear_statusmessage(int index)
  {
    if (index <= BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE) return;
    if (index >= BRIDGE_USERCLIENT_STATUS_MESSAGE__END__) return;

    statusmessage_[index][0] = '\0';
  }

  void
  CommonData::append_statusmessage(int index, const char* message)
  {
    if (index <= BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE) return;
    if (index >= BRIDGE_USERCLIENT_STATUS_MESSAGE__END__) return;

    strlcat(statusmessage_[index], message, sizeof(statusmessage_[index]));
  }

  void
  CommonData::send_notification_statusmessage(int index)
  {
    if (index <= BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE) return;
    if (index >= BRIDGE_USERCLIENT_STATUS_MESSAGE__END__) return;

    org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::send_notification_to_userspace(BRIDGE_USERCLIENT_NOTIFICATION_TYPE_STATUS_MESSAGE_UPDATED, index);
  }

  const char*
  CommonData::get_statusmessage(int index)
  {
    if (index <= BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE) return NULL;
    if (index >= BRIDGE_USERCLIENT_STATUS_MESSAGE__END__) return NULL;

    return statusmessage_[index];
  }

  void
  CommonData::increase_alloccount(void)
  {
    ++alloccount_;
    if (alloccount_ > 1024) {
      IOLOG_WARN("alloccount_ > 1024\n");
    }
    // IOLOG_DEVEL("CommonData::increase_alloccount alloccount_:%d\n", alloccount_);
  }

  void
  CommonData::decrease_alloccount(void)
  {
    --alloccount_;
    // IOLOG_DEVEL("CommonData::decrease_alloccount alloccount_:%d\n", alloccount_);
  }
}
