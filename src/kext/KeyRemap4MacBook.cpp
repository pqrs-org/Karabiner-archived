// -*- Mode: c++; indent-tabs-mode: nil; -*-

#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>
#include "base.hpp"
#include "KeyRemap4MacBook.hpp"
#include "Config.hpp"
#include "remap.hpp"
#include "RemapUtil.hpp"

org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard org_pqrs_driver_KeyRemap4MacBook::hookedKeyboard[MAXNUM_KEYBOARD];

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
  org_pqrs_KeyRemap4MacBook::sysctl_register();

  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    hookedKeyboard[i].kbd = NULL;
    hookedKeyboard[i].timer = NULL;
  }

  return res;
}

void
org_pqrs_driver_KeyRemap4MacBook::free(void)
{
  IOLog("KeyRemap4MacBook::free\n");

  org_pqrs_KeyRemap4MacBook::sysctl_unregister();
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
  bool res = super::start(provider);
  IOLog("KeyRemap4MacBook::start\n");
  if (!res) return res;

  workLoop       = IOWorkLoop::workLoop();

  keyboardNotifier = addNotification(gIOMatchedNotification,
                                     serviceMatching("IOHIKeyboard"),
                                     ((IOServiceNotificationHandler)&(org_pqrs_driver_KeyRemap4MacBook::notifier_hookKeyboard)),
                                     this, NULL, 0);
  if (keyboardNotifier == NULL) {
    IOLog("[KeyRemap4MacBook ERROR] addNotification(gIOMatchedNotification)\n");
    return false;
  }

  terminatedNotifier = addNotification(gIOTerminatedNotification,
                                       serviceMatching("IOHIKeyboard"),
                                       ((IOServiceNotificationHandler)&(org_pqrs_driver_KeyRemap4MacBook::notifier_unhookKeyboard)),
                                       this, NULL, 0);
  if (terminatedNotifier == NULL) {
    IOLog("[KeyRemap4MacBook ERROR] addNotification(gIOTerminatedNotification)\n");
    return false;
  }

  return res;
}

void
org_pqrs_driver_KeyRemap4MacBook::stop(IOService *provider)
{
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    hookedKeyboard[i].terminate(workLoop);
  }

  if (workLoop) {
    workLoop->release();
    workLoop = NULL;
  }

  if (keyboardNotifier) {
    keyboardNotifier->remove();
  }
  if (terminatedNotifier) {
    terminatedNotifier->remove();
  }

  IOLog("KeyRemap4MacBook::stop\n");
  super::stop(provider);
}

// ----------------------------------------------------------------------
void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::initialize(IOHIKeyboard *_kbd, IOWorkLoop *workLoop)
{
  kbd = _kbd;

  origEventAction = _kbd->_keyboardEventAction;
  kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(org_pqrs_driver_KeyRemap4MacBook::keyboardEventCallBack);

  if (workLoop != NULL) {
    timer = IOTimerEventSource::timerEventSource(reinterpret_cast<OSObject *>(this), reinterpret_cast<IOTimerEventSource::Action>(&(org_pqrs_driver_KeyRemap4MacBook::doKeyRepeat)));
    if (workLoop->addEventSource(timer) != kIOReturnSuccess) {
      timer->release();
      timer = NULL;
    }
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::terminate(IOWorkLoop *workLoop)
{
  if (kbd) {
    kbd->_keyboardEventAction = origEventAction;
    kbd = NULL;
  }
  if (timer) {
    if (workLoop) {
      workLoop->removeEventSource(timer);
    }
    timer->release();
    timer = NULL;
  }
}

// ----------------------------------------------------------------------
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard *
org_pqrs_driver_KeyRemap4MacBook::new_hookedKeyboard(void)
{
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    if (hookedKeyboard[i].kbd == NULL) {
      return hookedKeyboard + i;
    }
  }
  return NULL;
}

org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard *
org_pqrs_driver_KeyRemap4MacBook::search_hookedKeyboard(const IOHIKeyboard *kbd)
{
  if (kbd == NULL) {
    return NULL;
  }
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    if (hookedKeyboard[i].kbd == kbd) {
      return hookedKeyboard + i;
    }
  }
  return NULL;
}

// ----------------------------------------------------------------------
bool
org_pqrs_driver_KeyRemap4MacBook::notifier_hookKeyboard(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService)
{
  IOLog("KeyRemap4MacBook::notifier_hookKeyboard\n");

  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);
  return replaceKeyboardEvent(self, kbd);
}

bool
org_pqrs_driver_KeyRemap4MacBook::notifier_unhookKeyboard(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService)
{
  IOLog("KeyRemap4MacBook::notifier_unhookKeyboard\n");

  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);
  HookedKeyboard *p = search_hookedKeyboard(kbd);
  if (! p) return false;

  p->terminate(self->workLoop);
  return true;
}

bool
org_pqrs_driver_KeyRemap4MacBook::replaceKeyboardEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIKeyboard *kbd)
{
  if (kbd) {
    const char *name = kbd->getName();
    IOLog("KeyRemap4MacBook::replaceKeyboardEvent name = %s\n", name);

    // AppleADBKeyboard == PowerBook, IOHIKeyboard == MacBook, MacBook Pro, Mac mini, ...
    if (strcmp(name, "IOHIDKeyboard") == 0 ||
        strcmp(name, "AppleADBKeyboard") == 0) {
      HookedKeyboard *p = new_hookedKeyboard();
      if (p) {
        IOLog("KeyRemap4MacBook::replaceKeyboardEvent 0x%x (%x)\n", kbd, kbd->_keyboardEventAction);
        p->initialize(kbd, self->workLoop);
      }
    }
    return true;
  }
  return false;
}

// ----------------------------------------------------------------------
void
org_pqrs_driver_KeyRemap4MacBook::keyboardEventCallBack(OSObject *target,
                                                        unsigned int eventType,
                                                        unsigned int flags,
                                                        unsigned int key,
                                                        unsigned int charCode,
                                                        unsigned int charSet,
                                                        unsigned int origCharCode,
                                                        unsigned int origCharSet,
                                                        unsigned int keyboardType,
                                                        bool repeat,
                                                        AbsoluteTime ts,
                                                        OSObject *sender,
                                                        void *refcon)
{
  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, sender);
  if (kbd) {
    HookedKeyboard *p = search_hookedKeyboard(kbd);
    if (p) {
      // ------------------------------------------------------------
      // Because the key repeat generates it by oneself, I throw it away.
      if (repeat) {
        (p->repeat).ts = ts;
        return;
      }

      // ------------------------------------------------------------
      org_pqrs_KeyRemap4MacBook::listFireExtraKey.reset();

      bool ex_dropKey = false;
      org_pqrs_KeyRemap4MacBook::RemapParams params = {
        &eventType, &flags, &key, &charCode, &charSet,
        &origCharCode, &origCharSet, &keyboardType, &ts,
        &ex_dropKey, key,
      };
      org_pqrs_KeyRemap4MacBook::remap_core(params);

      // ignore KeyEvent if ex_dropKey is set in "remap_*".
      if (ex_dropKey) return;

      org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(org_pqrs_KeyRemap4MacBook::FireExtraKey::TYPE_BEFORE, p->origEventAction, target, params);

      p->origEventAction(target, eventType, flags, key, charCode,
                         charSet, origCharCode, origCharSet, keyboardType, repeat, ts);

      org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(org_pqrs_KeyRemap4MacBook::FireExtraKey::TYPE_AFTER, p->origEventAction, target, params);

      // set keyreeat
      if (eventType == org_pqrs_KeyRemap4MacBook::KeyEvent::DOWN) {
        (p->repeat).flags = flags;
        (p->repeat).key = key;
        (p->repeat).charCode = charCode;
        (p->repeat).origCharCode = origCharCode;
        (p->repeat).origCharSet = origCharSet;
        (p->repeat).keyboardType = keyboardType;
        (p->repeat).ts = ts;
        (p->repeat).target = target;

        int initial = org_pqrs_KeyRemap4MacBook::config.repeat_initial_wait;
        const int MINVAL = 200;
        if (initial < MINVAL) initial = MINVAL;
        p->timer->setTimeoutMS(initial);

      } else if (eventType == org_pqrs_KeyRemap4MacBook::KeyEvent::MODIFY || (p->repeat).key == key) {
        p->timer->cancelTimeout();
      }
    }
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::doKeyRepeat(OSObject *owner, IOTimerEventSource *sender)
{
  HookedKeyboard *p = reinterpret_cast<HookedKeyboard *>(owner);
  HookedKeyboard::RepeatInfo *r = &(p->repeat);

  p->origEventAction(r->target, org_pqrs_KeyRemap4MacBook::KeyEvent::DOWN, r->flags, r->key, r->charCode,
                     r->charSet, r->origCharCode, r->origCharSet, r->keyboardType, true, r->ts);

  int speed = org_pqrs_KeyRemap4MacBook::config.repeat_wait;
  const int MINVAL = 5;
  if (speed < MINVAL) speed = MINVAL;
  sender->setTimeoutMS(speed);
}
