#include <IOKit/IOLib.h>

#include "CommonData.hpp"
#include "GlobalLock.hpp"
#include "IOLogWrapper.hpp"
#include "TimerWrapper.hpp"

#define super    OSObject
OSDefineMetaClassAndStructors(org_pqrs_KeyRemap4MacBook_TimerWrapperObject, OSObject);

org_pqrs_KeyRemap4MacBook_TimerWrapperObject*
org_pqrs_KeyRemap4MacBook_TimerWrapperObject::timerEventSource(OSObject* owner, IOTimerEventSource::Action action)
{
  org_pqrs_KeyRemap4MacBook_TimerWrapperObject* p = new org_pqrs_KeyRemap4MacBook_TimerWrapperObject;

  if (p && ! p->init(owner, action)) {
    if (p) p->release();
    return NULL;
  }
  return p;
}

bool
org_pqrs_KeyRemap4MacBook_TimerWrapperObject::init(OSObject* owner, IOTimerEventSource::Action action)
{
  if (! super::init()) {
    return false;
  }

  owner_ = owner;
  action_ = action;
  active_ = false;

  return true;
}

void
org_pqrs_KeyRemap4MacBook_TimerWrapperObject::free(void)
{
  super::free();
  return;
}

#undef super

// ================================================================================
namespace org_pqrs_KeyRemap4MacBook {
  void
  TimerWrapper::initialize(IOWorkLoop* wl, OSObject* owner, IOTimerEventSource::Action func)
  {
    if (timer_) terminate();

    if (! wl) return;

    workloop_ = wl;

    object_ = org_pqrs_KeyRemap4MacBook_TimerWrapperObject::timerEventSource(owner, func);
    if (! object_) return;

    timer_ = IOTimerEventSource::timerEventSource(object_, callback_);

    if (workloop_->addEventSource(timer_) != kIOReturnSuccess) {
      IOLOG_ERROR("TimerWrapper addEventSource failed\n");
      timer_->release();
      timer_ = NULL;
    }
  }

  void
  TimerWrapper::terminate(void)
  {
    if (timer_) {
      timer_->cancelTimeout();
      if (workloop_) {
        workloop_->removeEventSource(timer_);
      }
      timer_->release();
      timer_ = NULL;
    }
    workloop_ = NULL;

    if (object_) {
      object_->release();
      object_ = NULL;
    }
  }

  IOReturn
  TimerWrapper::setTimeoutMS(UInt32 ms, bool overwrite)
  {
    if (! object_) return kIOReturnError;

    if (! timer_) {
      return kIOReturnNoResources;
    }
    if (! overwrite && object_->isActive()) {
      return kIOReturnSuccess;
    }

    object_->setActive(true);
    IOReturn retval = timer_->setTimeoutMS(ms);
    if (retval != kIOReturnSuccess) {
      IOLOG_WARN("setTimeoutMS is failed\n");
      object_->setActive(false);
    }
    return retval;
  }

  void
  TimerWrapper::cancelTimeout(void)
  {
    if (! object_) return;

    if (timer_) {
      timer_->cancelTimeout();
    }
    object_->setActive(false);
  }

  void
  TimerWrapper::callback_(OSObject* owner, IOTimerEventSource* sender)
  {
    GlobalLock::ScopedLock lk;
    if (! lk) return;

    org_pqrs_KeyRemap4MacBook_TimerWrapperObject* object = OSDynamicCast(org_pqrs_KeyRemap4MacBook_TimerWrapperObject, owner);
    if (! object) return;

    object->setActive(false);
    (object->getaction())(object->getowner(), sender);
  }
}
