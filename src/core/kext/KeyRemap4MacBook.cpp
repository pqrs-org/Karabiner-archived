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

namespace {
  int getconfig_repeat_initial_wait(void) {
    const int MINVAL = 200;
    int value = org_pqrs_KeyRemap4MacBook::config.repeat_initial_wait;
    if (value < MINVAL) return MINVAL;
    return value;
  }

  int getconfig_repeat_wait(void) {
    const int MINVAL = 5;
    int value = org_pqrs_KeyRemap4MacBook::config.repeat_wait;
    if (value < MINVAL) return MINVAL;
    return value;
  }

  int getconfig_keyoverlaidmodifier_initial_wait(void) {
    const int MINVAL = 200;
    int value = org_pqrs_KeyRemap4MacBook::config.repeat_keyoverlaidmodifier_initial_wait;
    if (value < MINVAL) return MINVAL;
    return value;
  }
}

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
    hookedKeyboard[i].extraRepeat.func = NULL;
  }
  for (int i = 0; i < MAXNUM_CONSUMER; ++i) {
    hookedConsumer[i].kbd = NULL;
  }
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    hookedPointing[i].pointing = NULL;
  }
  org_pqrs_KeyRemap4MacBook::clickWatcher.reset();
  org_pqrs_KeyRemap4MacBook::pressDownKeys.initialize();

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
  timer_refresh.initialize(workLoop, reinterpret_cast<OSObject *>(this), org_pqrs_driver_KeyRemap4MacBook::refreshHookedDevice);
  const int INTERVAL = 1000;
  timer_refresh.setTimeoutMS(INTERVAL);

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

  timer_refresh.terminate();

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
org_pqrs_driver_KeyRemap4MacBook::TimerWrapper::initialize(IOWorkLoop *_workLoop, OSObject *owner, IOTimerEventSource::Action func)
{
  if (timer) terminate();

  if (! _workLoop) return;

  workLoop = _workLoop;
  timer = IOTimerEventSource::timerEventSource(owner, func);

  if (workLoop->addEventSource(timer) != kIOReturnSuccess) {
    timer->release();
    timer = NULL;
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::TimerWrapper::terminate(void)
{
  if (timer) {
    timer->cancelTimeout();
    if (workLoop) {
      workLoop->removeEventSource(timer);
    }
    timer->release();
    timer = NULL;
  }
  workLoop = NULL;
}

// ----------------------------------------------------------------------
void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::initialize(IOHIKeyboard *_kbd, IOWorkLoop *workLoop)
{
  kbd = _kbd;
  refresh();

  timer_repeat.initialize(workLoop, reinterpret_cast<OSObject *>(this), org_pqrs_driver_KeyRemap4MacBook::doKeyRepeat);
  timer_extraRepeat.initialize(workLoop, reinterpret_cast<OSObject *>(this), org_pqrs_driver_KeyRemap4MacBook::doExtraKeyRepeat);
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
  timer_repeat.terminate();
  timer_extraRepeat.terminate();

  if (kbd) {
    if (getKeyboardEventCallback(kbd) == org_pqrs_driver_KeyRemap4MacBook::keyboardEventCallBack) {
      kbd->_keyboardEventAction = reinterpret_cast<KeyboardEventAction>(origEventCallback);
    }
    kbd = NULL;
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::setRepeatInfo(unsigned int eventType,
                                                                unsigned int flags, unsigned int key,
                                                                unsigned int charCode, unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet,
                                                                unsigned int keyboardType, AbsoluteTime ts,
                                                                OSObject *target, void *refcon)
{
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

    timer_repeat.setTimeoutMS(getconfig_repeat_initial_wait());

    if (org_pqrs_KeyRemap4MacBook::config.debug) {
      printf("KeyRemap4MacBook::setRepeatInfo set\n");
    }

  } else if (eventType == org_pqrs_KeyRemap4MacBook::KeyEvent::MODIFY || repeat.key == key) {
    timer_repeat.cancelTimeout();

    if (org_pqrs_KeyRemap4MacBook::config.debug) {
      printf("KeyRemap4MacBook::setRepeatInfo cancel\n");
    }
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::HookedKeyboard::setExtraRepeatInfo(org_pqrs_KeyRemap4MacBook::ExtraRepeatFunc::ExtraRepeatFunc func, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *target, void *refcon)
{
  if (func) {
    extraRepeat.flags = flags;
    extraRepeat.keyboardType = keyboardType;
    extraRepeat.ts = ts;
    extraRepeat.target = target;
    extraRepeat.sender = kbd;
    extraRepeat.refcon = refcon;
    extraRepeat.func = func;
    extraRepeat.counter = 0;
    timer_extraRepeat.setTimeoutMS(getconfig_keyoverlaidmodifier_initial_wait());

  } else {
    timer_extraRepeat.cancelTimeout();
    extraRepeat.func = NULL;
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

    origSpecialEventTarget = kbd->_keyboardSpecialEventTarget;

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

org_pqrs_driver_KeyRemap4MacBook::HookedConsumer *
org_pqrs_driver_KeyRemap4MacBook::get_1stHookedConsumer(void)
{
  for (int i = 0; i < MAXNUM_CONSUMER; ++i) {
    if (hookedConsumer[i].kbd) {
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
  for (int i = 0; i < MAXNUM_CONSUMER; ++i) {
    hookedConsumer[i].refresh();
  }
  for (int i = 0; i < MAXNUM_POINTING; ++i) {
    hookedPointing[i].refresh();
  }

  const int INTERVAL = 1000;
  if (sender) {
    sender->setTimeoutMS(INTERVAL);
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::cancelAllKeyRepeat(void)
{
  for (int i = 0; i < MAXNUM_KEYBOARD; ++i) {
    hookedKeyboard[i].timer_repeat.cancelTimeout();
    hookedKeyboard[i].timer_extraRepeat.cancelTimeout();
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
  return restoreKeyboardEvent(self, kbd);
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
  return restorePointingEvent(self, pointing);
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
      IOLog("KeyRemap4MacBook::replaceKeyboardSpecialEvent(Consumer) 0x%x (%x)\n", kbd, kbd->_keyboardSpecialEventAction);
      p->initialize(kbd);
    }

  } else {
    IOLog("KeyRemap4MacBook::replaceKeyboardEvent ignore device [%s]\n", name);
  }

  return true;
}

bool
org_pqrs_driver_KeyRemap4MacBook::restoreKeyboardEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIKeyboard *kbd)
{
  if (! kbd) return false;

  HookedKeyboard *pk = search_hookedKeyboard(kbd);
  if (pk) pk->terminate(self->workLoop);

  HookedConsumer *pc = search_hookedConsumer(kbd);
  if (pc) pc->terminate();

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

bool
org_pqrs_driver_KeyRemap4MacBook::restorePointingEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIPointing *pointing)
{
  if (! pointing) return false;

  HookedPointing *p = search_hookedPointing(pointing);
  if (p) p->terminate();

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
  if (! kbd) return;

  HookedKeyboard *p = search_hookedKeyboard(kbd);
  if (! p) return;

  // ------------------------------------------------------------
  // Because the key repeat generates it by oneself, I throw it away.
  if (repeat) {
    (p->repeat).ts = ts;
    (p->extraRepeat).ts = ts;
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
  org_pqrs_KeyRemap4MacBook::listFireConsumerKey.reset();
  org_pqrs_KeyRemap4MacBook::listFireRelativePointer.reset();

  unsigned int ex_extraRepeatFlags = 0;
  org_pqrs_KeyRemap4MacBook::ExtraRepeatFunc::ExtraRepeatFunc ex_extraRepeatFunc = NULL;

  org_pqrs_KeyRemap4MacBook::RemapUtil::normalizeKeyBeforeRemap(&key, &flags, keyboardType);

  org_pqrs_KeyRemap4MacBook::RemapParams params = {
    &eventType, &flags, &key, &charCode, &charSet,
    &origCharCode, &origCharSet, &keyboardType, &ts,
    key,
    KeyRemap4MacBook_bridge::ActiveApplicationInfo::UNKNOWN,
    &ex_extraRepeatFunc,
    &ex_extraRepeatFlags,
    (p->extraRepeat).counter,
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
    KeyRemap4MacBook_bridge::ActiveApplicationInfo::Reply activeApplicationInfo;
    int error = KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_ACTIVE_APPLICATION_INFO, NULL, 0, &activeApplicationInfo, sizeof(activeApplicationInfo));
    if (error == 0) {
      params.appType = activeApplicationInfo.type;
    }
    org_pqrs_KeyRemap4MacBook::remap_core(params);
  }

  // pointing emulation
  if (! org_pqrs_KeyRemap4MacBook::listFireRelativePointer.isEmpty()) {
    HookedPointing *hp = get_1stHookedPointing();
    if (hp) {
      org_pqrs_KeyRemap4MacBook::listFireRelativePointer.fire(hp->origRelativePointerEventCallback, hp->relativePointerEventTarget, hp->pointing, ts);
    }
  }

  // consumer emulation
  if (! org_pqrs_KeyRemap4MacBook::listFireConsumerKey.isEmpty()) {
    HookedConsumer *hc = get_1stHookedConsumer();
    if (hc) {
      org_pqrs_KeyRemap4MacBook::listFireConsumerKey.fire(hc->origSpecialEventCallback, hc->origSpecialEventTarget, ts, sender, refcon);
    }
  }

  if (p->origEventCallback) {
    org_pqrs_KeyRemap4MacBook::RemapUtil::fireKey(p->origEventCallback,
                                                  target, eventType, flags, key, charCode,
                                                  charSet, origCharCode, origCharSet, keyboardType, repeat,
                                                  ts, sender, refcon);
    org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(p->origEventCallback, target, charSet, origCharCode, origCharSet, keyboardType, ts, sender, refcon);
  }

  p->setExtraRepeatInfo(ex_extraRepeatFunc, ex_extraRepeatFlags, keyboardType, ts, target, refcon);
  p->setRepeatInfo(eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType, ts, target, refcon);

  if (org_pqrs_KeyRemap4MacBook::allFlagStatus.numHeldDownKeys <= 0) {
    org_pqrs_KeyRemap4MacBook::RemapUtil::fireModifiers(0, p->origEventCallback, target, keyboardType, ts, sender, refcon);
    org_pqrs_KeyRemap4MacBook::pressDownKeys.clear(p->origEventCallback, target, ts, sender, refcon);
  }
}

void
org_pqrs_driver_KeyRemap4MacBook::doKeyRepeat(OSObject *owner, IOTimerEventSource *sender)
{
  HookedKeyboard *p = reinterpret_cast<HookedKeyboard *>(owner);
  HookedKeyboard::RepeatInfo *r = &(p->repeat);

  if (p->origEventCallback) {
    org_pqrs_KeyRemap4MacBook::RemapUtil::fireKey(p->origEventCallback,
                                                  r->target, org_pqrs_KeyRemap4MacBook::KeyEvent::DOWN, r->flags, r->key, r->charCode,
                                                  r->charSet, r->origCharCode, r->origCharSet, r->keyboardType, true, r->ts, r->sender, r->refcon);
  }

  sender->setTimeoutMS(getconfig_repeat_wait());
}

void
org_pqrs_driver_KeyRemap4MacBook::doExtraKeyRepeat(OSObject *owner, IOTimerEventSource *sender)
{
  HookedKeyboard *p = reinterpret_cast<HookedKeyboard *>(owner);
  HookedKeyboard::ExtraRepeatInfo *r = &(p->extraRepeat);

  if (! r->func) return;

  if (p->origEventCallback) {
    r->func(p->origEventCallback, r->target, r->flags, r->keyboardType, r->ts, r->sender, r->refcon);
  }
  ++(r->counter);

  sender->setTimeoutMS(getconfig_repeat_wait());
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
  if (! kbd) return;

  HookedConsumer *p = search_hookedConsumer(kbd);
  if (! p) return;

  // ------------------------------------------------------------
  if (p->origSpecialEventCallback) {
    if (org_pqrs_KeyRemap4MacBook::config.debug) {
      printf("caught keyboardSpecialEventCallBack: eventType %d, flags 0x%x, key %d, flavor %d, guid %d\n", eventType, flags, key, flavor, guid);
    }

    org_pqrs_KeyRemap4MacBook::listFireExtraKey.reset();

    org_pqrs_KeyRemap4MacBook::KeyCode::KeyCode ex_remapKeyCode = org_pqrs_KeyRemap4MacBook::KeyCode::NONE;
    org_pqrs_KeyRemap4MacBook::RemapConsumerParams params = {
      &eventType, &flags, &key, &flavor, &ts, &ex_remapKeyCode,
    };
    org_pqrs_KeyRemap4MacBook::remap_consumer(params);

    // ----------------------------------------
    HookedKeyboard *hk = get_1stHookedKeyboard();
    unsigned int charCode = 0;
    unsigned int charSet = 0;
    unsigned origCharCode = 0;
    unsigned origCharSet = 0;
    unsigned int keyboardType = org_pqrs_KeyRemap4MacBook::KeyboardType::MACBOOK;

    if (hk) {
      if (ex_remapKeyCode != org_pqrs_KeyRemap4MacBook::KeyCode::NONE) {
        if (hk->origEventCallback) {
          hk->origEventCallback(hk->origEventTarget, eventType, flags, ex_remapKeyCode, charCode, charSet, origCharCode, origCharSet, keyboardType, false, ts, hk->kbd, NULL);
        }
        hk->setRepeatInfo(eventType, flags, ex_remapKeyCode, charCode, charSet, origCharCode, origCharSet, keyboardType, ts, hk->origEventTarget, NULL);
      }

      org_pqrs_KeyRemap4MacBook::listFireExtraKey.fire(hk->origEventCallback, hk->origEventTarget, charSet, origCharCode, origCharSet, keyboardType, ts, sender, NULL);
    }

    org_pqrs_KeyRemap4MacBook::RemapUtil::fireConsumer(p->origSpecialEventCallback,
                                                       target, eventType, flags, key, flavor, guid, repeat, ts, sender, refcon);
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
  if (! pointing) return;

  HookedPointing *p = search_hookedPointing(pointing);
  if (! p) return;

  // ------------------------------------------------------------
  org_pqrs_KeyRemap4MacBook::listFireRelativePointer.reset();

  if (org_pqrs_KeyRemap4MacBook::config.debug_pointing) {
    printf("caught relativePointerEventCallBack: buttons: %d, dx: %d, dy: %d, ts: 0x%x\n", buttons, dx, dy, ts);
  }

  if (buttons) {
    cancelAllKeyRepeat();
  }

  bool ex_dropEvent = false;
  org_pqrs_KeyRemap4MacBook::Params_RelativePointerEventCallback params = {
    target, buttons, dx, dy, ts, sender, refcon,
  };
  org_pqrs_KeyRemap4MacBook::RemapPointingParams_relative remapParams = {
    &params, &ex_dropEvent,
  };
  org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(remapParams);

  RelativePointerEventCallback reCallback = p->origRelativePointerEventCallback;

  if (! ex_dropEvent) {
    org_pqrs_KeyRemap4MacBook::RemapUtil::execCallBack_RelativePointerEventCallBack(reCallback, params);
  }

  if (! org_pqrs_KeyRemap4MacBook::listFireRelativePointer.isEmpty()) {
    org_pqrs_KeyRemap4MacBook::listFireRelativePointer.fire(reCallback, target, pointing, ts);
  }

  if (org_pqrs_KeyRemap4MacBook::firePointingScroll.isEnable()) {
    org_pqrs_KeyRemap4MacBook::firePointingScroll.fire(p->origScrollWheelEventCallback, p->scrollWheelEventTarget, pointing, ts);
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
  if (! pointing) return;

  HookedPointing *p = search_hookedPointing(pointing);
  if (! p) return;

  // ------------------------------------------------------------
  cancelAllKeyRepeat();

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
