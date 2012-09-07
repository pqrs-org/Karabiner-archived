#include "UserClient_kext.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "RemapClass.hpp"
#include "util/GlobalLock.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedConsumer.hpp"
#include "util/ListHookedPointing.hpp"
#include "strlcpy_utf8.hpp"

#define super IOUserClient

OSDefineMetaClassAndStructors(org_pqrs_driver_KeyRemap4MacBook_UserClient_kext, IOUserClient)

OSAsyncReference64 org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::asyncref_;
bool org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::notification_enabled_ = false;

IOExternalMethodDispatch org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::methods_[BRIDGE_USERCLIENT__END__] = {
  { // BRIDGE_USERCLIENT_OPEN
    reinterpret_cast<IOExternalMethodAction>(&static_callback_open), // Method pointer.
    0,                                                               // No scalar input values.
    0,                                                               // No struct input value.
    0,                                                               // No scalar output values.
    0                                                                // No struct output value.
  },
  { // BRIDGE_USERCLIENT_CLOSE
    reinterpret_cast<IOExternalMethodAction>(&static_callback_close), // Method pointer.
    0,                                                         // No scalar input values.
    0,                                                         // No struct input value.
    0,                                                         // No scalar output values.
    0                                                          // No struct output value.
  },
  { // BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION
    reinterpret_cast<IOExternalMethodAction>(&static_callback_synchronized_communication), // Method pointer.
    0,                                                                                     // No scalar input values.
    sizeof(BridgeUserClientStruct),                                                        // The size of the input struct.
    1,                                                                                     // One scalar output value.
    0,                                                                                     // No struct output value.
  },
  { // BRIDGE_USERCLIENT_NOTIFICATION_FROM_KEXT
    reinterpret_cast<IOExternalMethodAction>(&static_callback_notification_from_kext), // Method pointer.
    0,                                                                                 // No scalar input values.
    0,                                                                                 // No struct input value.
    0,                                                                                 // No scalar output values.
    0                                                                                  // No struct output value.
  }
};

// ============================================================
// initWithTask is called as a result of the user process calling IOServiceOpen.
bool
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::initWithTask(task_t owningTask, void* securityToken, UInt32 type)
{
  if (clientHasPrivilege(owningTask, kIOClientPrivilegeLocalUser) != KERN_SUCCESS) {
    IOLOG_ERROR("UserClient_kext::initWithTask clientHasPrivilege failed\n");
    return false;
  }

  if (! super::initWithTask(owningTask, securityToken, type)) {
    IOLOG_ERROR("UserClient_kext::initWithTask super::initWithTask failed\n");
    return false;
  }

  task_     = owningTask;
  provider_ = NULL;

  // Don't change static values here. (For example, notification_enabled_)
  // initWithTask is called by each IOServiceOpen.
  //
  // If IOService is opened, other client will be failed.
  // Changing static values by other IOServiceOpen may destroy the current connection.

  return true;
}

// start is called after initWithTask as a result of the user process calling IOServiceOpen.
bool
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::start(IOService* provider)
{
  provider_ = OSDynamicCast(org_pqrs_driver_KeyRemap4MacBook, provider);
  if (! provider_) {
    IOLOG_ERROR("UserClient_kext::start provider == NULL\n");
    return false;
  }

  // It's important not to call super::start if some previous condition
  // (like an invalid provider) would cause this function to return false.
  // I/O Kit won't call stop on an object if its start function returned false.
  if (! super::start(provider)) {
    IOLOG_ERROR("UserClient_kext::start super::start failed\n");
    return false;
  }

  return true;
}

void
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::stop(IOService* provider)
{
  super::stop(provider);
  provider_ = NULL;
}

// clientClose is called as a result of the user process calling IOServiceClose.
IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::clientClose(void)
{
  // Defensive coding in case the user process called IOServiceClose
  // without calling BRIDGE_USERCLIENT_CLOSE first.
  callback_close();

  // Inform the user process that this user client is no longer available. This will also cause the
  // user client instance to be destroyed.
  //
  // terminate would return false if the user process still had this user client open.
  // This should never happen in our case because this code path is only reached if the user process
  // explicitly requests closing the connection to the user client.
  if (! terminate()) {
    IOLOG_ERROR("UserClient_kext::clientClose terminate() failed\n");
  }

  // DON'T call super::clientClose, which just returns kIOReturnUnsupported.
  return kIOReturnSuccess;
}

bool
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::didTerminate(IOService* provider, IOOptionBits options, bool* defer)
{
  // If all pending I/O has been terminated, close our provider. If I/O is still outstanding, set defer to true
  // and the user client will not have stop called on it.
  callback_close();
  *defer = false;

  return super::didTerminate(provider, options, defer);
}

IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::externalMethod(uint32_t selector, IOExternalMethodArguments* arguments,
                                                                 IOExternalMethodDispatch* dispatch, OSObject* target, void* reference)
{
  if (selector < static_cast<uint32_t>(BRIDGE_USERCLIENT__END__)) {
    dispatch = &(methods_[selector]);

    if (! target) {
      target = this;
    }
  }

  return super::externalMethod(selector, arguments, dispatch, target, reference);
}


// ======================================================================
IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::static_callback_open(org_pqrs_driver_KeyRemap4MacBook_UserClient_kext* target, void* reference, IOExternalMethodArguments* arguments)
{
  if (! target) return kIOReturnBadArgument;
  return target->callback_open();
}

IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::callback_open(void)
{
  org_pqrs_KeyRemap4MacBook::GlobalLock::ScopedLock lk;
  if (! lk) return kIOReturnCannotLock;

  if (provider_ == NULL || isInactive()) {
    // Return an error if we don't have a provider. This could happen if the user process
    // called callback_open without calling IOServiceOpen first. Or, the user client could be
    // in the process of being terminated and is thus inactive.
    IOLOG_ERROR("UserClient_kext::callback_open kIOReturnNotAttached\n");
    return kIOReturnNotAttached;
  }

  if (! provider_->open(this)) {
    // The most common reason this open call will fail is because the provider is already open
    // and it doesn't support being opened by more than one client at a time.
    IOLOG_ERROR("UserClient_kext::callback_open kIOReturnExclusiveAccess\n");
    return kIOReturnExclusiveAccess;
  }

  return kIOReturnSuccess;
}

// ----------------------------------------------------------------------
IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::static_callback_close(org_pqrs_driver_KeyRemap4MacBook_UserClient_kext* target, void* reference, IOExternalMethodArguments* arguments)
{
  if (! target) return kIOReturnBadArgument;
  return target->callback_close();
}

IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::callback_close(void)
{
  org_pqrs_KeyRemap4MacBook::GlobalLock::ScopedLock lk;
  if (! lk) return kIOReturnCannotLock;

  if (! provider_) {
    // Return an error if we don't have a provider. This could happen if the user process
    // called callback_close without calling IOServiceOpen first.
    IOLOG_ERROR("UserClient_kext::callback_close kIOReturnNotAttached\n");
    return kIOReturnNotAttached;
  }

  if (! provider_->isOpen(this)) {
    // This error can occur in the normal usage. (by fast user switching)
    // So we suppress the log message.
#if 0
    IOLOG_ERROR("UserClient_kext::callback_close kIOReturnNotOpen\n");
#endif
    return kIOReturnNotOpen;
  }

  notification_enabled_ = false;

  org_pqrs_KeyRemap4MacBook::Config::set_initialized(false);

  // Make sure we're the one who opened our provider before we tell it to close.
  provider_->close(this);

  return kIOReturnSuccess;
}

// ----------------------------------------------------------------------
IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::static_callback_synchronized_communication(org_pqrs_driver_KeyRemap4MacBook_UserClient_kext* target, void* reference, IOExternalMethodArguments* arguments)
{
  if (! target) return kIOReturnBadArgument;
  return target->callback_synchronized_communication(static_cast<const BridgeUserClientStruct*>(arguments->structureInput), &arguments->scalarOutput[0]);
}

IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::callback_synchronized_communication(const BridgeUserClientStruct* inputdata, uint64_t* outputdata)
{
  org_pqrs_KeyRemap4MacBook::GlobalLock::ScopedLock lk;
  if (! lk) return kIOReturnCannotLock;

  IOReturn result = kIOReturnSuccess;
  IOMemoryDescriptor* memorydescriptor = NULL;

  if (! inputdata || ! outputdata) {
    result = kIOReturnBadArgument;
    IOLOG_ERROR("UserClient_kext::callback_synchronized_communication kIOReturnBadArgument\n");
    goto finish;
  }

  if (provider_ == NULL || isInactive()) {
    // Return an error if we don't have a provider. This could happen if the user process
    // called callback_synchronized_communication without calling IOServiceOpen first.
    // Or, the user client could be in the process of being terminated and is thus inactive.
    result = kIOReturnNotAttached;
    IOLOG_ERROR("UserClient_kext::callback_synchronized_communication kIOReturnNotAttached\n");
    goto finish;
  }

  if (! provider_->isOpen(this)) {
    // Return an error if we do not have the driver open. This could happen if the user process
    // did not call callback_open before calling this function.
    result = kIOReturnNotOpen;
    IOLOG_ERROR("UserClient_kext::callback_synchronized_communication kIOReturnNotOpen\n");
    goto finish;
  }

  memorydescriptor = IOMemoryDescriptor::withAddressRange(inputdata->data, inputdata->size, kIODirectionNone, task_);
  if (! memorydescriptor) {
    result = kIOReturnVMError;
    IOLOG_ERROR("UserClient_kext::callback_synchronized_communication kIOReturnVMError\n");
    goto finish;
  }

  // wire it and make sure we can write it
  result = memorydescriptor->prepare(kIODirectionOutIn);
  if (kIOReturnSuccess != result) {
    IOLOG_ERROR("UserClient_kext::callback_synchronized_communication IOMemoryDescriptor::prepare failed(0x%x)\n", result);
    goto finish;
  }

  {
    // this map() will create a mapping in the users (the client of this IOUserClient) address space.
    IOMemoryMap* memorymap = memorydescriptor->map();
    if (! memorymap) {
      result = kIOReturnVMError;
      IOLOG_ERROR("UserClient_kext::callback_synchronized_communication IOMemoryDescriptor::map failed\n");

    } else {
      mach_vm_address_t address = memorymap->getAddress();
      handle_synchronized_communication(inputdata->type, inputdata->option, address, inputdata->size, outputdata);
      memorymap->release();
    }
  }

  // Done with the I/O now.
  memorydescriptor->complete(kIODirectionOutIn);

finish:
  if (memorydescriptor) {
    memorydescriptor->release();
  }

  return result;
}

// ------------------------------------------------------------
IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::static_callback_notification_from_kext(org_pqrs_driver_KeyRemap4MacBook_UserClient_kext* target, void* reference, IOExternalMethodArguments* arguments)
{
  if (! target) return kIOReturnBadArgument;
  return target->callback_notification_from_kext(arguments->asyncReference);
}

IOReturn
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::callback_notification_from_kext(OSAsyncReference64 asyncReference)
{
  org_pqrs_KeyRemap4MacBook::GlobalLock::ScopedLock lk;
  if (! lk) return kIOReturnCannotLock;

  if (provider_ == NULL || isInactive()) {
    // Return an error if we don't have a provider. This could happen if the user process
    // called callback_notification_from_kext without calling IOServiceOpen first.
    // Or, the user client could be in the process of being terminated and is thus inactive.
    IOLOG_ERROR("UserClient_kext::callback_notification_from_kext kIOReturnNotAttached\n");
    return kIOReturnNotAttached;
  }

  if (! provider_->isOpen(this)) {
    // Return an error if we do not have the driver open. This could happen if the user process
    // did not call callback_open before calling this function.
    IOLOG_ERROR("UserClient_kext::callback_notification_from_kext kIOReturnNotOpen\n");
    return kIOReturnNotOpen;
  }

  // ----------------------------------------
  bcopy(asyncReference, asyncref_, sizeof(OSAsyncReference64));
  notification_enabled_ = true;
  return kIOReturnSuccess;
}

void
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::send_notification_to_userspace(uint32_t type, uint32_t option)
{
  if (notification_enabled_) {
    io_user_reference_t args[] = { type, option };
    sendAsyncResult64(asyncref_, kIOReturnSuccess, args, 2);
  }
}

// ------------------------------------------------------------
void
org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::handle_synchronized_communication(uint32_t type, uint32_t option, mach_vm_address_t address, mach_vm_size_t size, uint64_t* outputdata)
{
  *outputdata = BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_ERROR_GENERIC;

  switch (type) {
    case BRIDGE_USERCLIENT_TYPE_SET_REMAPCLASSES_INITIALIZE_VECTOR:
    {
      org_pqrs_KeyRemap4MacBook::Config::set_initialized(false);

      const uint32_t* initialize_vector = reinterpret_cast<uint32_t*>(address);
      if (initialize_vector) {
        if (org_pqrs_KeyRemap4MacBook::RemapClassManager::load_remapclasses_initialize_vector(initialize_vector, size)) {
          *outputdata = BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_SUCCESS;
        }
      }
      break;
    }

    case BRIDGE_USERCLIENT_TYPE_SET_CONFIG:
    {
      const int32_t* config = reinterpret_cast<int32_t*>(address);
      if (config) {
        if (org_pqrs_KeyRemap4MacBook::RemapClassManager::set_config(config, size)) {
          org_pqrs_KeyRemap4MacBook::Config::set_initialized(true);
          *outputdata = BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_SUCCESS;
        }
      }
      break;
    }

    case BRIDGE_USERCLIENT_TYPE_GET_STATUS_MESSAGE:
    {
      const char* statusmessage = org_pqrs_KeyRemap4MacBook::CommonData::get_statusmessage(option);
      char* p = reinterpret_cast<char*>(address);

      if (statusmessage && p) {
        pqrs::strlcpy_utf8::strlcpy(p, statusmessage, static_cast<size_t>(size));
        *outputdata = BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_SUCCESS;
      }
      break;
    }

    case BRIDGE_USERCLIENT_TYPE_SET_WORKSPACEDATA:
    {
      if (size != sizeof(BridgeWorkSpaceData)) {
        IOLOG_ERROR("BRIDGE_USERCLIENT_TYPE_SET_ESSENTIAL_CONFIG wrong 'size' parameter\n");
      } else {
        const BridgeWorkSpaceData* p = reinterpret_cast<const BridgeWorkSpaceData*>(address);
        if (p) {
          org_pqrs_KeyRemap4MacBook::CommonData::setcurrent_workspacedata(*p);
          *outputdata = BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_SUCCESS;
        }
      }
      break;
    }

    case BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_KEYBOARD:
    case BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_CONSUMER:
    case BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_POINTING:
    {
      if (size != sizeof(BridgeDeviceInformation)) {
        IOLOG_ERROR("BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_* wrong 'size' parameter\n");
      } else {
        BridgeDeviceInformation* p = reinterpret_cast<BridgeDeviceInformation*>(address);
        if (p) {
          switch (type) {
            case BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_KEYBOARD:
              org_pqrs_KeyRemap4MacBook::ListHookedKeyboard::instance().getDeviceInformation(*p, option);
              break;
            case BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_CONSUMER:
              org_pqrs_KeyRemap4MacBook::ListHookedConsumer::instance().getDeviceInformation(*p, option);
              break;
            case BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_POINTING:
              org_pqrs_KeyRemap4MacBook::ListHookedPointing::instance().getDeviceInformation(*p, option);
              break;
          }
          *outputdata = BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_SUCCESS;
        }
      }

      break;
    }
  }
}
