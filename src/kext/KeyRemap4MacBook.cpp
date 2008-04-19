// -*- Mode: c++; indent-tabs-mode: nil; -*-

#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>
#include "base.hpp"
#include "KeyRemap4MacBook.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "remap.hpp"
#include "RemapUtil.hpp"

org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard org_pqrs_driver_KeyRemap4MacBook::hookedKeyboard[MAXNUM_KEYBOARD];
org_pqrs_driver_KeyRemap4MacBook::HookedConsumer org_pqrs_driver_KeyRemap4MacBook::hookedConsumer[MAXNUM_CONSUMER];
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
  for (int i = 0; i < MAXNUM_CONSUMER; ++i) {
    hookedConsumer[i].kbd = NULL;
  }
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    hookedPointing[i].pointing = NULL;
  }
  org_pqrs_KeyRemap4MacBook::clickWatcher.reset();

  KeyRemap4MacBook_client::initialize();

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

  workLoop = IOWorkLoop::workLoop();
  if (workLoop) {
    refreshTimer = IOTimerEventSource::timerEventSource(reinterpret_cast<OSObject *>(this), reinterpret_cast<IOTimerEventSource::Action>(&(org_pqrs_driver_KeyRemap4MacBook::refreshHookedDevice)));
    if (workLoop->addEventSource(refreshTimer) != kIOReturnSuccess) {
      refreshTimer->release();
      refreshTimer = NULL;
    }
    const int INTERVAL = 1000;
    refreshTimer->setTimeoutMS(INTERVAL);
  }

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

  if (refreshTimer) {
    refreshTimer->cancelTimeout();
    if (workLoop) {
      workLoop->removeEventSource(refreshTimer);
    }
    refreshTimer->release();
    refreshTimer = NULL;
  }
  if (workLoop) {
    workLoop->release();
    workLoop = NULL;
  }

  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    hookedKeyboard[i].terminate(workLoop);
  }
  for (int i = 0; i < MAXNUM_CONSUMER; ++i) {
    hookedConsumer[i].terminate();
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
  refresh();

  if (workLoop != NULL) {
    timer = IOTimerEventSource::timerEventSource(reinterpret_cast<OSObject *>(this), reinterpret_cast<IOTimerEventSource::Action>(&(org_pqrs_driver_KeyRemap4MacBook::doKeyRepeat)));
    if (workLoop->addEventSource(timer) != kIOReturnSuccess) {
      timer->release();
      timer = NULL;
    }
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::refresh(void)
{
  if (kbd == NULL) return;

  KeyboardEventCallback oldCallback = getKeyboardEventCallback(kbd);
  KeyboardEventCallback newCallback = org_pqrs_driver_KeyRemap4MacBook::keyboardEventCallBack;

  if (oldCallback != newCallback) {
    origEventCallback = oldCallback;
    kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(newCallback);

    origEventTarget = kbd->_keyboardEventTarget;

    IOLog("KeyRemap4MacBook::HookedKeyboard::refresh KeyboardEventAction\n");
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::terminate(IOWorkLoop *workLoop)
{
  if (kbd) {
    if (getKeyboardEventCallback(kbd) == org_pqrs_driver_KeyRemap4MacBook::keyboardEventCallBack) {
      kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(origEventCallback);
    }
    kbd = NULL;
  }
  if (timer) {
    timer->cancelTimeout();
    if (workLoop) {
      workLoop->removeEventSource(timer);
    }
    timer->release();
    timer = NULL;
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::setRepeatInfo(unsigned int eventType,
                                                                unsigned int flags, unsigned int key,
                                                                unsigned int charCode, unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet,
                                                                unsigned int keyboardType, AbsoluteTime ts,
                                                                OSObject *target, void *refcon)
{
  if (! timer) return;

  // set keyreeat
  if (eventType == org_pqrs_KeyRemap4MacBook::KeyEvent::DOWN) {
    repeat.flags = flags;
    repeat.key = key;
    repeat.charCode = charCode;
    repeat.charSet = charSet;
    repeat.origCharCode = origCharCode;
    repeat.origCharSet = origCharSet;
    repeat.keyboardType = keyboardType;
    repeat.ts = ts;
    repeat.target = target;
    repeat.sender = kbd;
    repeat.refcon = refcon;

    int initial = org_pqrs_KeyRemap4MacBook::config.repeat_initial_wait;
    const int MINVAL = 200;
    if (initial < MINVAL) initial = MINVAL;
    timer->setTimeoutMS(initial);

    if (org_pqrs_KeyRemap4MacBook::config.debug) {
      printf("KeyRemap4MacBook::setRepeatInfo set\n");
    }

  } else if (eventType == org_pqrs_KeyRemap4MacBook::KeyEvent::MODIFY || repeat.key == key) {
    timer->cancelTimeout();

    if (org_pqrs_KeyRemap4MacBook::config.debug) {
      printf("KeyRemap4MacBook::setRepeatInfo cancel\n");
    }
  }
}

// --------------------
void
org_pqrs_driver_KeyRemap4MacBook::HookedConsumer::initialize(IOHIKeyboard *_kbd)
{
  kbd = _kbd;
  refresh();
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedConsumer::refresh(void)
{
  if (kbd == NULL) return;

  KeyboardSpecialEventCallback oldCallback = getKeyboardSpecialEventCallback(kbd);
  KeyboardSpecialEventCallback newCallback = org_pqrs_driver_KeyRemap4MacBook::keyboardSpecialEventCallBack;
  if (oldCallback != newCallback) {
    origSpecialEventCallback = oldCallback;
    kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(newCallback);

    IOLog("KeyRemap4MacBook::HookedConsumer::refresh KeyboardSpecialEventAction\n");
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedConsumer::terminate(void)
{
  if (kbd) {
    if (getKeyboardSpecialEventCallback(kbd) == org_pqrs_driver_KeyRemap4MacBook::keyboardSpecialEventCallBack) {
      kbd->_keyboardSpecialEventAction = reinterpret_cast<KeyboardSpecialEventAction>(origSpecialEventCallback);
    }
    kbd = NULL;
  }
}

// --------------------
void
org_pqrs_driver_KeyRemap4MacBook::HookedPointing::initialize(IOHIPointing *_pointing)
{
  pointing = _pointing;
  refresh();
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedPointing::refresh(void)
{
  if (pointing == NULL) return;

  RelativePointerEventCallback oldReCallback = getRelativePointerEventCallback(pointing);
  RelativePointerEventCallback newReCallback = org_pqrs_driver_KeyRemap4MacBook::relativePointerEventCallBack;
  if (oldReCallback != newReCallback) {
    origRelativePointerEventCallback = oldReCallback;
    pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(newReCallback);
    relativePointerEventTarget = pointing->_relativePointerEventTarget;
    IOLog("KeyRemap4MacBook::HookedPointing::refresh RelativePointerEventAction\n");
  }

  ScrollWheelEventCallback oldSwCallback = getScrollWheelEventCallback(pointing);
  ScrollWheelEventCallback newSwCallback = org_pqrs_driver_KeyRemap4MacBook::scrollWheelEventCallback;
  if (oldSwCallback != newSwCallback) {
    origScrollWheelEventCallback = oldSwCallback;
    pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(newSwCallback);
    scrollWheelEventTarget = pointing->_scrollWheelEventTarget;
    IOLog("KeyRemap4MacBook::HookedPointing::refresh ScrollWheelEventAction\n");
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedPointing::terminate(void)
{
  if (pointing) {
    if (getRelativePointerEventCallback(pointing) == org_pqrs_driver_KeyRemap4MacBook::relativePointerEventCallBack) {
      pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(origRelativePointerEventCallback);
    }
    if (getScrollWheelEventCallback(pointing) == org_pqrs_driver_KeyRemap4MacBook::scrollWheelEventCallback) {
      pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(origScrollWheelEventCallback);
    }
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

org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard *
org_pqrs_driver_KeyRemap4MacBook::get_1stHookedKeyboard(void)
{
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    if (hookedKeyboard[i].kbd) {
      return hookedKeyboard + i;
    }
  }
  return NULL;
}

// --------------------
org_pqrs_driver_KeyRemap4MacBook::HookedConsumer *
org_pqrs_driver_KeyRemap4MacBook::new_hookedConsumer(void)
{
  for (int i = 0; i < MAXNUM_CONSUMER; ++i) {
    if (hookedConsumer[i].kbd == NULL) {
      return hookedConsumer + i;
    }
  }
  return NULL;
}

org_pqrs_driver_KeyRemap4MacBook::HookedConsumer *
org_pqrs_driver_KeyRemap4MacBook::search_hookedConsumer(const IOHIKeyboard *kbd)
{
  if (kbd == NULL) {
    return NULL;
  }
  for (int i = 0; i < MAXNUM_CONSUMER; ++i) {
    if (hookedConsumer[i].kbd == kbd) {
      return hookedConsumer + i;
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
void
org_pqrs_driver_KeyRemap4MacBook::refreshHookedDevice(OSObject *owner, IOTimerEventSource *sender)
{
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    hookedKeyboard[i].refresh();
  }
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    hookedPointing[i].refresh();
  }

  const int INTERVAL = 1000;
  if (sender) {
    sender->setTimeoutMS(INTERVAL);
  }
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

  } else if (strcmp(name, "IOHIDConsumer") == 0) {
    HookedConsumer *p = new_hookedConsumer();
    if (p) {
      IOLog("KeyRemap4MacBook::replaceKeyboardEvent(Consumer) 0x%x (%x)\n", kbd, kbd->_keyboardEventAction);
      p->initialize(kbd);
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

  // IOHIDPointing == External USB Mouse, AppleUSBGrIIITrackpad == MacBook Pro, AppleADBMouseType4 == PowerBook
  if (strcmp(name, "IOHIDPointing") == 0 ||
      strcmp(name, "AppleUSBGrIIITrackpad") == 0 ||
      strcmp(name, "AppleADBMouseType4") == 0) {
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
      if (org_pqrs_KeyRemap4MacBook::config.general_capslock_led_hack) {
        int led = p->kbd->getLEDStatus();
        if (led == 0) {
          p->kbd->setAlphaLockFeedback(true);
        }
      }

      // ------------------------------------------------------------
      org_pqrs_KeyRemap4MacBook::listFireExtraKey.reset();
      org_pqrs_KeyRemap4MacBook::listFirePointingClick.reset();

      bool ex_dropKey = false;
      KeyRemap4MacBook_bridge::ActiveApplicationInfo::Reply activeApplicationInfo;
      org_pqrs_KeyRemap4MacBook::RemapParams params = {
        &eventType, &flags, &key, &charCode, &charSet,
        &origCharCode, &origCharSet, &keyboardType, &ts,
        &ex_dropKey, key,
        &activeApplicationInfo,
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
        int error = KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_ACTIVE_APPLICATION_INFO, NULL, 0, &activeApplicationInfo, sizeof(activeApplicationInfo));
        if (error) {
          activeApplicationInfo.reset();
        }
        org_pqrs_KeyRemap4MacBook::remap_core(params);
      }

      // pointing emulation
      if (! org_pqrs_KeyRemap4MacBook::listFirePointingClick.isEmpty()) {
        HookedPointing *hp = get_1stHookedPointing();
        if (hp) {
          org_pqrs_KeyRemap4MacBook::listFirePointingClick.fire(hp->origRelativePointerEventCallback, hp->relativePointerEventTarget, hp->pointing, ts);
        }
      }

      if (p->origEventCallback) {
        org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(org_pqrs_KeyRemap4MacBook::FireExtraKey::TYPE_BEFORE, p->origEventCallback, target, charSet, origCharCode, origCharSet, ts, sender, refcon);

        if (! ex_dropKey) {
          p->origEventCallback(target, eventType, flags, key, charCode,
                               charSet, origCharCode, origCharSet, keyboardType, repeat, ts, sender, refcon);
        }

        org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(org_pqrs_KeyRemap4MacBook::FireExtraKey::TYPE_AFTER, p->origEventCallback, target, charSet, origCharCode, origCharSet, ts, sender, refcon);
      }
      if (ex_dropKey) return;

      p->setRepeatInfo(eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType, ts, target, refcon);
    }
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::doKeyRepeat(OSObject *owner, IOTimerEventSource *sender)
{
  HookedKeyboard *p = reinterpret_cast<HookedKeyboard *>(owner);
  HookedKeyboard::RepeatInfo *r = &(p->repeat);

  if (p->origEventCallback) {
    p->origEventCallback(r->target, org_pqrs_KeyRemap4MacBook::KeyEvent::DOWN, r->flags, r->key, r->charCode,
                         r->charSet, r->origCharCode, r->origCharSet, r->keyboardType, true, r->ts, r->sender, r->refcon);
  }

  int speed = org_pqrs_KeyRemap4MacBook::config.repeat_wait;
  const int MINVAL = 5;
  if (speed < MINVAL) speed = MINVAL;
  sender->setTimeoutMS(speed);
}

// --------------------
void
org_pqrs_driver_KeyRemap4MacBook::keyboardSpecialEventCallBack(OSObject *target,
                                                               unsigned int eventType,
                                                               unsigned int flags,
                                                               unsigned int key,
                                                               unsigned int flavor,
                                                               UInt64 guid,
                                                               bool repeat,
                                                               AbsoluteTime ts,
                                                               OSObject *sender,
                                                               void *refcon)
{
  IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, sender);
  if (kbd) {
    HookedConsumer *p = search_hookedConsumer(kbd);
    if (p) {
      if (p->origSpecialEventCallback) {
        if (org_pqrs_KeyRemap4MacBook::config.debug) {
          printf("caught keyboardSpecialEventCallBack: eventType %d, flags 0x%x, key %d, flavor %d\n", eventType, flags, key, flavor);
        }

        org_pqrs_KeyRemap4MacBook::listFireExtraKey.reset();

        bool ex_dropKey = false;
        bool ex_remapKey = false;
        unsigned int ex_remapKeyCode = 0;
        org_pqrs_KeyRemap4MacBook::RemapConsumerParams params = {
          &eventType, &flags, &key, &flavor, &ts, &ex_dropKey, &ex_remapKey, &ex_remapKeyCode,
        };
        org_pqrs_KeyRemap4MacBook::remap_consumer(params);

        // ----------------------------------------
        HookedKeyboard *hk = get_1stHookedKeyboard();
        unsigned int charCode = 0;
        unsigned int charSet = 0;
        unsigned origCharCode = 0;
        unsigned origCharSet = 0;
        unsigned int keyboardType = org_pqrs_KeyRemap4MacBook::KeyboardType::MACBOOK;

        org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(org_pqrs_KeyRemap4MacBook::FireExtraKey::TYPE_BEFORE, hk->origEventCallback, hk->origEventTarget, charSet, origCharCode, origCharSet, ts, sender, NULL);

        if (ex_remapKey) {
          if (hk->origEventCallback) {
            hk->origEventCallback(hk->origEventTarget, eventType, flags, ex_remapKeyCode, charCode, charSet, origCharCode, origCharSet, keyboardType, false, ts, hk->kbd, NULL);
          }
          hk->setRepeatInfo(eventType, flags, ex_remapKeyCode, charCode, charSet, origCharCode, origCharSet, keyboardType, ts, hk->origEventTarget, NULL);
        }

        org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(org_pqrs_KeyRemap4MacBook::FireExtraKey::TYPE_AFTER, hk->origEventCallback, hk->origEventTarget, charSet, origCharCode, origCharSet, ts, sender, NULL);

        if (! ex_dropKey) {
          p->origSpecialEventCallback(target, eventType, flags, key, flavor, guid, repeat, ts, sender, refcon);
        }
      }
    }
  }
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
      org_pqrs_KeyRemap4MacBook::listFirePointingClick.reset();

      if (org_pqrs_KeyRemap4MacBook::config.debug_pointing) {
        printf("caught relativePointerEventCallBack: buttons: %d, dx: %d, dy: %d, ts: 0x%x\n", buttons, dx, dy, ts);
      }

      bool ex_dropEvent = false;
      org_pqrs_KeyRemap4MacBook::RemapPointingParams_relative params = {
        &buttons, &dx, &dy, &ts, &ex_dropEvent,
      };
      org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(params);

      RelativePointerEventCallback reCallback = p->origRelativePointerEventCallback;

      if (! ex_dropEvent) {
        if (reCallback) {
          if (org_pqrs_KeyRemap4MacBook::config.debug_pointing) {
            printf("sending relativePointerEventCallBack: buttons: %d, dx: %d, dy: %d, ts: 0x%x\n", buttons, dx, dy, ts);
          }
          reCallback(target, buttons, dx, dy, ts, sender, refcon);
        }
      }

      if (! org_pqrs_KeyRemap4MacBook::listFirePointingClick.isEmpty()) {
        org_pqrs_KeyRemap4MacBook::listFirePointingClick.fire(reCallback, target, pointing, ts);
      }

      if (org_pqrs_KeyRemap4MacBook::firePointingScroll.isEnable()) {
        org_pqrs_KeyRemap4MacBook::firePointingScroll.fire(p->origScrollWheelEventCallback, p->scrollWheelEventTarget, pointing, ts);
      }
    }
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::scrollWheelEventCallback(OSObject * target,
                                                           short      deltaAxis1,
                                                           short      deltaAxis2,
                                                           short      deltaAxis3,
                                                           IOFixed    fixedDelta1,
                                                           IOFixed    fixedDelta2,
                                                           IOFixed    fixedDelta3,
                                                           SInt32     pointDelta1,
                                                           SInt32     pointDelta2,
                                                           SInt32     pointDelta3,
                                                           SInt32     options,
                                                           AbsoluteTime ts,
                                                           OSObject * sender,
                                                           void *     refcon)
{
  IOHIPointing *pointing = OSDynamicCast(IOHIPointing, sender);
  if (pointing) {
    HookedPointing *p = search_hookedPointing(pointing);
    if (p) {
      if (org_pqrs_KeyRemap4MacBook::config.debug_pointing) {
        printf("scrollWheelEventCallback: deltaAxis(%d, %d, %d), fixedDelta(%d, %d, %d), pointDelta(%d,%d,%d), options: %d\n",
               deltaAxis1, deltaAxis2, deltaAxis3,
               fixedDelta1, fixedDelta2, fixedDelta3,
               pointDelta1, pointDelta2, pointDelta3,
               options);
      }

      ScrollWheelEventCallback callback = p->origScrollWheelEventCallback;
      if (callback) {
        callback(target,
                 deltaAxis1, deltaAxis2, deltaAxis3,
                 fixedDelta1, fixedDelta2, fixedDelta3,
                 pointDelta1, pointDelta2, pointDelta3,
                 options, ts, sender, refcon);
      }
    }
  }
}
