// -*- Mode: c++; indent-tabs-mode: nil; -*-

#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>
#include "base.hpp"
#include "KeyRemap4MacBook.hpp"
#include "Config.hpp"
#include "remap.hpp"
#include "RemapUtil.hpp"

org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard org_pqrs_driver_KeyRemap4MacBook::hookedKeyboard[MAXNUM_KEYBOARD];
org_pqrs_driver_KeyRemap4MacBook::HookedPointing org_pqrs_driver_KeyRemap4MacBook::hookedPointing[MAXNUM_POINTING];

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
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    hookedPointing[i].pointing = NULL;
  }
  org_pqrs_KeyRemap4MacBook::clickWatcher.reset();

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

  // ----------------------------------------
  notifier_hookKeyboard = addNotification(gIOMatchedNotification,
                                          serviceMatching("IOHIKeyboard"),
                                          ((IOServiceNotificationHandler)&(org_pqrs_driver_KeyRemap4MacBook::notifierfunc_hookKeyboard)),
                                          this, NULL, 0);
  if (notifier_hookKeyboard == NULL) {
    IOLog("[KeyRemap4MacBook ERROR] addNotification(gIOMatchedNotification) Keyboard\n");
    return false;
  }

  notifier_unhookKeyboard = addNotification(gIOTerminatedNotification,
                                            serviceMatching("IOHIKeyboard"),
                                            ((IOServiceNotificationHandler)&(org_pqrs_driver_KeyRemap4MacBook::notifierfunc_unhookKeyboard)),
                                            this, NULL, 0);
  if (notifier_unhookKeyboard == NULL) {
    IOLog("[KeyRemap4MacBook ERROR] addNotification(gIOTerminatedNotification) Keyboard\n");
    return false;
  }

  // ----------------------------------------
  notifier_hookPointing = addNotification(gIOMatchedNotification,
                                          serviceMatching("IOHIPointing"),
                                          ((IOServiceNotificationHandler)&(org_pqrs_driver_KeyRemap4MacBook::notifierfunc_hookPointing)),
                                          this, NULL, 0);
  if (notifier_hookPointing == NULL) {
    IOLog("[KeyRemap4MacBook ERROR] addNotification(gIOMatchedNotification) Pointing\n");
    return false;
  }

  notifier_unhookPointing = addNotification(gIOTerminatedNotification,
                                            serviceMatching("IOHIPointing"),
                                            ((IOServiceNotificationHandler)&(org_pqrs_driver_KeyRemap4MacBook::notifierfunc_unhookPointing)),
                                            this, NULL, 0);
  if (notifier_unhookPointing == NULL) {
    IOLog("[KeyRemap4MacBook ERROR] addNotification(gIOTerminatedNotification) Pointing\n");
    return false;
  }

  return res;
}

void
org_pqrs_driver_KeyRemap4MacBook::stop(IOService *provider)
{
  if (notifier_hookKeyboard) {
    notifier_hookKeyboard->remove();
  }
  if (notifier_unhookKeyboard) {
    notifier_unhookKeyboard->remove();
  }
  if (notifier_hookPointing) {
    notifier_hookPointing->remove();
  }
  if (notifier_unhookPointing) {
    notifier_unhookPointing->remove();
  }

  if (workLoop) {
    workLoop->release();
    workLoop = NULL;
  }

  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    hookedKeyboard[i].terminate(workLoop);
  }
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    hookedPointing[i].terminate();
  }

  IOLog("KeyRemap4MacBook::stop\n");
  super::stop(provider);
}

// ----------------------------------------------------------------------
void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::initialize(IOHIKeyboard *_kbd, IOWorkLoop *workLoop)
{
  kbd = _kbd;

  origEventCallback = reinterpret_cast<KeyboardEventCallback>(_kbd->_keyboardEventAction);
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
    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(origEventCallback);
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

// --------------------
void
org_pqrs_driver_KeyRemap4MacBook::HookedPointing::initialize(IOHIPointing *_pointing)
{
  pointing = _pointing;

  origRelativePointerEventCallback = reinterpret_cast<RelativePointerEventCallback>(_pointing->_relativePointerEventAction);
  relativePointerEventTarget = _pointing->_relativePointerEventTarget;
  pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(org_pqrs_driver_KeyRemap4MacBook::relativePointerEventCallBack);

  origScrollWheelEventCallback = reinterpret_cast<ScrollWheelEventCallback>(_pointing->_scrollWheelEventAction);
  scrollWheelEventTarget = _pointing->_scrollWheelEventTarget;
  pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(org_pqrs_driver_KeyRemap4MacBook::scrollWheelEventCallback);
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedPointing::terminate(void)
{
  if (pointing) {
    pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(origRelativePointerEventCallback);
    pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(origScrollWheelEventCallback);
    pointing = NULL;
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

// --------------------
org_pqrs_driver_KeyRemap4MacBook::HookedPointing *
org_pqrs_driver_KeyRemap4MacBook::new_hookedPointing(void)
{
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    if (hookedPointing[i].pointing == NULL) {
      return hookedPointing + i;
    }
  }
  return NULL;
}

org_pqrs_driver_KeyRemap4MacBook::HookedPointing *
org_pqrs_driver_KeyRemap4MacBook::search_hookedPointing(const IOHIPointing *pointing)
{
  if (pointing == NULL) {
    return NULL;
  }
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    if (hookedPointing[i].pointing == pointing) {
      return hookedPointing + i;
    }
  }
  return NULL;
}

org_pqrs_driver_KeyRemap4MacBook::HookedPointing *
org_pqrs_driver_KeyRemap4MacBook::get_1stHookedPointing(void)
{
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    if (hookedPointing[i].pointing) {
      return hookedPointing + i;
    }
  }
  return NULL;
}

// ----------------------------------------------------------------------
bool
org_pqrs_driver_KeyRemap4MacBook::notifierfunc_hookKeyboard(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService)
{
  IOLog("KeyRemap4MacBook::notifierfunc_hookKeyboard\n");

  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);
  return replaceKeyboardEvent(self, kbd);
}

bool
org_pqrs_driver_KeyRemap4MacBook::notifierfunc_unhookKeyboard(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService)
{
  IOLog("KeyRemap4MacBook::notifierfunc_unhookKeyboard\n");

  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);
  HookedKeyboard *p = search_hookedKeyboard(kbd);
  if (! p) return false;

  p->terminate(self->workLoop);
  return true;
}

bool
org_pqrs_driver_KeyRemap4MacBook::notifierfunc_hookPointing(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService)
{
  IOLog("KeyRemap4MacBook::notifierfunc_hookPointing\n");

  IOHIPointing *pointing = OSDynamicCast(IOHIPointing, newService);
  return replacePointingEvent(self, pointing);
}

bool
org_pqrs_driver_KeyRemap4MacBook::notifierfunc_unhookPointing(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService)
{
  IOLog("KeyRemap4MacBook::notifierfunc_unhookPointing\n");

  IOHIPointing *pointing = OSDynamicCast(IOHIPointing, newService);
  HookedPointing *p = search_hookedPointing(pointing);
  if (! p) return false;

  p->terminate();
  return true;
}

// --------------------------------------------------
bool
org_pqrs_driver_KeyRemap4MacBook::replaceKeyboardEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIKeyboard *kbd)
{
  if (! kbd) return false;

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
  } else {
    IOLog("KeyRemap4MacBook::replaceKeyboardEvent ignore device [%s]\n", name);
  }

  return true;
}

bool
org_pqrs_driver_KeyRemap4MacBook::replacePointingEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIPointing *pointing)
{
  if (! pointing) return false;

  const char *name = pointing->getName();
  IOLog("KeyRemap4MacBook::replacePointingEvent name = %s\n", name);

  // IOHIDPointing == External USB Mouse, AppleUSBGrIIITrackpad == MacBook Pro, ...
  if (strcmp(name, "IOHIDPointing") == 0 ||
      strcmp(name, "AppleUSBGrIIITrackpad") == 0) {
    HookedPointing *p = new_hookedPointing();
    if (p) {
      IOLog("KeyRemap4MacBook::replacePointingEvent 0x%x (%x)\n", pointing);
      p->initialize(pointing);
    }
  } else {
    IOLog("KeyRemap4MacBook::replacePointingEvent ignore device [%s]\n", name);
  }

  return true;
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

      bool skip = false;
      if (org_pqrs_KeyRemap4MacBook::config.general_dont_remap_internal &&
          org_pqrs_KeyRemap4MacBook::RemapUtil::isInternalKeyboard(keyboardType)) {
        skip = true;
      }
      if (org_pqrs_KeyRemap4MacBook::config.general_dont_remap_external &&
          ! org_pqrs_KeyRemap4MacBook::RemapUtil::isInternalKeyboard(keyboardType)) {
        skip = true;
      }
      if (! skip) {
        org_pqrs_KeyRemap4MacBook::remap_core(params);
      }

      // pointing emulation
      if (org_pqrs_KeyRemap4MacBook::firePointingClick.isEnable()) {
        doClick(org_pqrs_KeyRemap4MacBook::firePointingClick.getButton(), ts);
        org_pqrs_KeyRemap4MacBook::firePointingClick.unset();
      }

      // ignore KeyEvent if ex_dropKey is set in "remap_*".
      if (ex_dropKey) return;

      org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(org_pqrs_KeyRemap4MacBook::FireExtraKey::TYPE_BEFORE, p->origEventCallback, target, sender, refcon, params);

      p->origEventCallback(target, eventType, flags, key, charCode,
                           charSet, origCharCode, origCharSet, keyboardType, repeat, ts, sender, refcon);

      org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(org_pqrs_KeyRemap4MacBook::FireExtraKey::TYPE_AFTER, p->origEventCallback, target, sender, refcon, params);

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
        (p->repeat).sender = sender;
        (p->repeat).refcon = refcon;

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

  p->origEventCallback(r->target, org_pqrs_KeyRemap4MacBook::KeyEvent::DOWN, r->flags, r->key, r->charCode,
                       r->charSet, r->origCharCode, r->origCharSet, r->keyboardType, true, r->ts, r->sender, r->refcon);

  int speed = org_pqrs_KeyRemap4MacBook::config.repeat_wait;
  const int MINVAL = 5;
  if (speed < MINVAL) speed = MINVAL;
  sender->setTimeoutMS(speed);
}

// --------------------
void
org_pqrs_driver_KeyRemap4MacBook::relativePointerEventCallBack(OSObject *target,
                                                               int buttons,
                                                               int dx,
                                                               int dy,
                                                               AbsoluteTime ts,
                                                               OSObject *sender,
                                                               void *refcon)
{
  IOHIPointing *pointing = OSDynamicCast(IOHIPointing, sender);
  if (pointing) {
    HookedPointing *p = search_hookedPointing(pointing);
    if (p) {
      if (org_pqrs_KeyRemap4MacBook::config.debug_pointing) {
        printf("relativePointerEventCallBack: buttons: %d, dx: %d, dy: %d, ts: 0x%x\n", buttons, dx, dy, ts);
      }

      bool dropEvent = false;
      if (org_pqrs_KeyRemap4MacBook::allFlagStatus.fn.isHeldDown()) {
        int deltaAxis1 = (-dy * org_pqrs_KeyRemap4MacBook::config.pointing_relative2scroll_ratio) / 1000;
        int deltaAxis2 = (-dx * org_pqrs_KeyRemap4MacBook::config.pointing_relative2scroll_ratio) / 1000;

        org_pqrs_KeyRemap4MacBook::firePointingScroll.set(deltaAxis1, deltaAxis2, 0);
        doScroll(ts);
        dropEvent = true;
      }

      if (buttons != org_pqrs_KeyRemap4MacBook::PointingButton::NONE) {
        org_pqrs_KeyRemap4MacBook::clickWatcher.click();
      }

      if (! dropEvent) {
        p->origRelativePointerEventCallback(target, buttons, dx, dy, ts, sender, refcon);
      }
    }
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::doClick(int buttons, AbsoluteTime ts)
{
  HookedPointing *p = get_1stHookedPointing();
  if (p) {
    p->origRelativePointerEventCallback(p->relativePointerEventTarget, buttons, 0, 0, ts, p->pointing, 0);
  }
}

// ----------
void
org_pqrs_driver_KeyRemap4MacBook::scrollWheelEventCallback(OSObject *target,
                                                           short int deltaAxis1,
                                                           short int deltaAxis2,
                                                           short int deltaAxis3,
                                                           IOFixed fixedDelta1,
                                                           IOFixed fixedDelta2,
                                                           IOFixed fixedDelta3,
                                                           SInt32 pointDelta1,
                                                           SInt32 pointDelta2,
                                                           SInt32 pointDelta3,
                                                           SInt32 options,
                                                           AbsoluteTime ts,
                                                           OSObject *sender,
                                                           void *refcon)
{
  IOHIPointing *pointing = OSDynamicCast(IOHIPointing, sender);
  if (pointing) {
    HookedPointing *p = search_hookedPointing(pointing);
    if (p) {
      if (org_pqrs_KeyRemap4MacBook::config.debug_pointing) {
        printf("scrollWheelEventCallback: d[%d %d %d] f[%d %d %d] p[%d %d %d] %d\n",
               deltaAxis1, deltaAxis2, deltaAxis3,
               fixedDelta1, fixedDelta2, fixedDelta3,
               pointDelta1, pointDelta2, pointDelta3,
               options);
      }

      p->origScrollWheelEventCallback(target, deltaAxis1, deltaAxis2, deltaAxis3, fixedDelta1, fixedDelta2, fixedDelta3, pointDelta1, pointDelta2, pointDelta3, options, ts, sender, refcon);
    }
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::doScroll(AbsoluteTime ts)
{
  HookedPointing *p = get_1stHookedPointing();
  if (p) {
    short int deltaAxis1 = org_pqrs_KeyRemap4MacBook::firePointingScroll.getDeltaAxis1();
    short int deltaAxis2 = org_pqrs_KeyRemap4MacBook::firePointingScroll.getDeltaAxis2();
    short int deltaAxis3 = org_pqrs_KeyRemap4MacBook::firePointingScroll.getDeltaAxis3();
    IOFixed fixedDelta1 = deltaAxis1 << 16;
    IOFixed fixedDelta2 = deltaAxis2 << 16;
    IOFixed fixedDelta3 = deltaAxis3 << 16;
    p->origScrollWheelEventCallback(p->scrollWheelEventTarget,
                                    deltaAxis1, deltaAxis2, deltaAxis3,
                                    fixedDelta1, fixedDelta2, fixedDelta3,
                                    deltaAxis1, deltaAxis2, deltaAxis3,
                                    0, ts, p->pointing, 0);
  }
}
