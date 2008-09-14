#include "Driver.hpp"
#include "Core.hpp"

// ----------------------------------------------------------------------
// http://developer.apple.com/documentation/DeviceDrivers/Conceptual/WritingDeviceDriver/CPluPlusRuntime/chapter_2_section_3.html

// This convention makes it easy to invoke base class member functions.
#define super    IOService
// You cannot use the "super" macro here, however, with the
//  OSDefineMetaClassAndStructors macro.
OSDefineMetaClassAndStructors(org_pqrs_driver_KeyRemap4MacBook, IOService)

// ----------------------------------------------------------------------
bool
org_pqrs_driver_KeyRemap4MacBook::init(OSDictionary *dict)
{
  IOLog("KeyRemap4MacBook::init\n");

  bool res = super::init(dict);
  org_pqrs_KeyRemap4MacBook::Core::initialize();

  return res;
}

void
org_pqrs_driver_KeyRemap4MacBook::free(void)
{
  IOLog("KeyRemap4MacBook::free\n");

  org_pqrs_KeyRemap4MacBook::Core::terminate();

  super::free();
}

IOService *
org_pqrs_driver_KeyRemap4MacBook::probe(IOService *provider, SInt32 *score)
{
  IOService *res = super::probe(provider, score);
  return res;
}

bool
org_pqrs_driver_KeyRemap4MacBook::start(IOService *provider)
{
  IOLog("KeyRemap4MacBook::start\n");

  bool res = super::start(provider);
  if (!res) return res;

  org_pqrs_KeyRemap4MacBook::Core::start();

  return res;
}

void
org_pqrs_driver_KeyRemap4MacBook::stop(IOService *provider)
{
  IOLog("KeyRemap4MacBook::stop\n");

  org_pqrs_KeyRemap4MacBook::Core::stop();

  super::stop(provider);
}
