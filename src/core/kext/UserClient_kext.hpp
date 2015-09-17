#ifndef USERCLIENT_KEXT_HPP
#define USERCLIENT_KEXT_HPP

#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOUserClient.h>
END_IOKIT_INCLUDE;

#include "Driver.hpp"
#include "bridge.h"

#define KEXT_CLASSNAME org_pqrs_driver_Karabiner
#define USERCLIENT_KEXT_CLASSNAME org_pqrs_driver_Karabiner_UserClient_kext

class USERCLIENT_KEXT_CLASSNAME final : public IOUserClient {
  OSDeclareDefaultStructors(USERCLIENT_KEXT_CLASSNAME);

public:
  // IOUserClient methods
  virtual bool initWithTask(task_t owningTask, void* securityToken, UInt32 type) override;

  virtual bool start(IOService* provider) override;
  virtual void stop(IOService* provider) override;

  virtual IOReturn clientClose(void) override;

  virtual bool didTerminate(IOService* provider, IOOptionBits options, bool* defer) override;

  static void send_notification_to_userspace(uint32_t type, uint32_t option);

protected:
  virtual IOReturn externalMethod(uint32_t selector, IOExternalMethodArguments* arguments,
                                  IOExternalMethodDispatch* dispatch, OSObject* target, void* reference) override;

private:
  // ------------------------------------------------------------
  static IOReturn static_callback_open(USERCLIENT_KEXT_CLASSNAME* target, void* reference, IOExternalMethodArguments* arguments);
  IOReturn callback_open(uint64_t bridge_version_app, uint64_t* outputdata);

  static IOReturn static_callback_close(USERCLIENT_KEXT_CLASSNAME* target, void* reference, IOExternalMethodArguments* arguments);
  IOReturn callback_close(void);

  static IOReturn static_callback_synchronized_communication(USERCLIENT_KEXT_CLASSNAME* target, void* reference, IOExternalMethodArguments* arguments);
  IOReturn callback_synchronized_communication(const BridgeUserClientStruct* inputdata, uint64_t* outputdata);

  static IOReturn static_callback_notification_from_kext(USERCLIENT_KEXT_CLASSNAME* target, void* reference, IOExternalMethodArguments* arguments);
  IOReturn callback_notification_from_kext(OSAsyncReference64 asyncReference);

  // ------------------------------------------------------------
  void handle_synchronized_communication(uint32_t type, uint32_t option, uint8_t* buffer, size_t size, uint64_t* outputdata);

  // ------------------------------------------------------------
  KEXT_CLASSNAME* provider_;
  static IOExternalMethodDispatch methods_[BRIDGE_USERCLIENT__END__];
  static OSAsyncReference64 asyncref_;
  static bool notification_enabled_;
};

#endif
