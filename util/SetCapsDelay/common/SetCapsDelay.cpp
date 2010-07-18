// -*- Mode: c++; indent-tabs-mode: nil; -*-

#include "base.hpp"
#include "SetCapsDelay.hpp"

// ----------------------------------------------------------------------
// http://developer.apple.com/documentation/DeviceDrivers/Conceptual/WritingDeviceDriver/CPluPlusRuntime/chapter_2_section_3.html

// This convention makes it easy to invoke base class member functions.
#define super    IOService
// You cannot use the "super" macro here, however, with the
//  OSDefineMetaClassAndStructors macro.
OSDefineMetaClassAndStructors(org_pqrs_driver_SetCapsDelay, IOService)

// ----------------------------------------------------------------------
org_pqrs_driver_SetCapsDelay::Item org_pqrs_driver_SetCapsDelay::item[org_pqrs_driver_SetCapsDelay::MAXNUM];

bool
org_pqrs_driver_SetCapsDelay::init(OSDictionary* dict)
{
  IOLOG_INFO("init\n");
  bool res = super::init(dict);

  for (int i = 0; i < MAXNUM; ++i) {
    item[i].service = NULL;
  }

  return res;
}

void
org_pqrs_driver_SetCapsDelay::free(void)
{
  IOLOG_INFO("free\n");
  super::free();
}

IOService*
org_pqrs_driver_SetCapsDelay::probe(IOService* provider, SInt32* score)
{
  IOService* res = super::probe(provider, score);
  return res;
}

bool
org_pqrs_driver_SetCapsDelay::start(IOService* provider)
{
  IOLOG_INFO("start\n");
  bool res = super::start(provider);
  if (! res) { return res; }

  notifier_hookKeyboard_ = addMatchingNotification(gIOMatchedNotification,
                                                   serviceMatching("IOHIDKeyboard"),
                                                   org_pqrs_driver_SetCapsDelay::notifierfunc_hookKeyboard,
                                                   this, NULL, 0);
  if (notifier_hookKeyboard_ == NULL) {
    IOLOG_ERROR("notifier_hookKeyboard_ == NULL\n");
    return false;
  }

  return res;
}

void
org_pqrs_driver_SetCapsDelay::stop(IOService* provider)
{
  IOLOG_INFO("stop\n");
  if (notifier_hookKeyboard_) notifier_hookKeyboard_->remove();

  for (int i = 0; i < MAXNUM; ++i) {
    IOHIDEventService* s = item[i].service;
    if (s && s->_reserved) {
      IOLOG_INFO("setDelay service:%p %d->%d\n", s, static_cast<int>(s->_reserved->capsDelayMS), item[i].delayms);
      s->_reserved->capsDelayMS = item[i].delayms;
    }
    item[i].service = NULL;
  }

  super::stop(provider);
}

// ----------------------------------------
bool
org_pqrs_driver_SetCapsDelay::notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier)
{
  IOHIDKeyboard* device = OSDynamicCast(IOHIDKeyboard, newService);
  if (! device) {
    IOLOG_ERROR("setDelay device == NULL\n");
    return false;
  }

  IOHIDEventService* service = device->_provider;
  if (! service) return false;
  if (! service->_reserved) return false;

  for (int i = 0; i < MAXNUM; ++i) {
    if (item[i].service) continue;

    item[i].service = service;
    item[i].delayms = service->_reserved->capsDelayMS;
    break;
  }

  enum {
    NEW_CAPS_DELAY_MS = 500,
  };
  IOLOG_INFO("setDelay service:%p %d->%d\n", service, static_cast<int>(service->_reserved->capsDelayMS), NEW_CAPS_DELAY_MS);
  service->_reserved->capsDelayMS = NEW_CAPS_DELAY_MS;

  return true;
}
