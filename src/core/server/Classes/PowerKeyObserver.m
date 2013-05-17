// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
//
// This code is based on PowerKey of Peter Kamb.
// https://github.com/pkamb/PowerKey/

#import <IOKit/hidsystem/ev_keymap.h>
#import <IOKit/hidsystem/IOLLEvent.h>
#import "ClientForKernelspace.h"
#import "PowerKeyObserver.h"

@implementation PowerKeyObserver

@synthesize eventTap;
@synthesize enqueued;
@synthesize shouldBlockPowerKeyKeyCode;
@synthesize savedPowerButtonEvent;
@synthesize clientForKernelspace;
@synthesize ignoreNext_POWER_KEY_TYPE_SUBTYPE;

enum {
  POWER_KEY_TYPE_NONE,
  POWER_KEY_TYPE_SUBTYPE, // subtype == NX_SUBTYPE_POWER_KEY
  POWER_KEY_TYPE_KEYCODE, // subtype == NX_SUBTYPE_AUX_CONTROL_BUTTONS and keyCode == NX_POWER_KEY
};

// The power button sends two events.
// POWER_KEY_TYPE_SUBTYPE and POWER_KEY_TYPE_KEYCODE.
//
// A build-in keyboard of MacBook sends these events.
// - POWER_KEY_TYPE_SUBTYPE (at key down)
// - POWER_KEY_TYPE_KEYCODE (at key down)
//
// An external keyboard which has power key sends these events.
// - POWER_KEY_TYPE_SUBTYPE (at key down)
// - POWER_KEY_TYPE_KEYCODE (at key down)
// - POWER_KEY_TYPE_KEYCODE (at key up)

- (int) getPowerKeyType:(CGEventRef)cgEvent
{
  if (! cgEvent) return POWER_KEY_TYPE_NONE;

  NSEvent* event = [NSEvent eventWithCGEvent:cgEvent];
  if (! event) return POWER_KEY_TYPE_NONE;

  if ([event type] != NSSystemDefined) return POWER_KEY_TYPE_NONE;

  if ([event subtype] == NX_SUBTYPE_POWER_KEY) {
    return POWER_KEY_TYPE_SUBTYPE;
  }

  if ([event subtype] == NX_SUBTYPE_AUX_CONTROL_BUTTONS) {
    int keyCode = (([event data1] & 0xFFFF0000) >> 16);
    if (keyCode == NX_POWER_KEY) {
      return POWER_KEY_TYPE_KEYCODE;
    }
  }

  return POWER_KEY_TYPE_NONE;
}

static CGEventRef eventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon)
{
  PowerKeyObserver* self = (PowerKeyObserver*)(refcon);
  if (! self) return event;

  switch (type) {
    case kCGEventTapDisabledByTimeout:
      // Re-enable the event tap if it times out.
      CGEventTapEnable([self eventTap], true);
      break;

    case NSSystemDefined:
    {
      if ([[self clientForKernelspace] has_consumer]) {
        switch ([self getPowerKeyType:event]) {
          case POWER_KEY_TYPE_SUBTYPE:
            if (self.ignoreNext_POWER_KEY_TYPE_SUBTYPE) {
              self.ignoreNext_POWER_KEY_TYPE_SUBTYPE = NO;
              return event;
            }

            // This event show a shutdown dialog.
            if (! self.enqueued) {
              self.enqueued = YES;

              // Save the original event from here because
              // enqueued power key might have different modifierFlags.
              //
              // For example, control-eject sends a power button event without any modifierflags.
              // But, enqueued power key sends a power button event with control modifierflags.
              // We need to send the original event when power button is not changed.
              if (self.savedPowerButtonEvent) {
                CFRelease(self.savedPowerButtonEvent);
              }
              self.savedPowerButtonEvent = CGEventCreateCopy(event);

              self.shouldBlockPowerKeyKeyCode = YES;
              [[self clientForKernelspace] enqueue_power_key];
              event = NULL;

            } else {
              self.enqueued = NO;

              if ([[self clientForKernelspace] is_power_key_changed]) {
                self.shouldBlockPowerKeyKeyCode = YES;
                event = NULL;
              } else {
                self.shouldBlockPowerKeyKeyCode = NO;

                if (self.savedPowerButtonEvent) {
                  event = self.savedPowerButtonEvent;
                  // Do not call CFRelease for self.savedPowerButtonEvent.
                  // It will be released by Quartz Event Services.
                  self.savedPowerButtonEvent = NULL;
                }
              }
            }
            break;

          case POWER_KEY_TYPE_KEYCODE:
            if (self.shouldBlockPowerKeyKeyCode) {
              event = NULL;
            }
            break;

          case POWER_KEY_TYPE_NONE:
            // do nothing
            break;
        }
      }

      break;
    }
  }

  return event;
}

- (void) start
{
  if (eventTap) return;

  // We need to grab *only* NSSystemDefined events.
  //
  // But, if we pass only NSSystemDefinedMask to CGEventTapCreate,
  // we cannot select objects on Adobe Fireworks.
  // (It might be a bug of OS X or Fireworks...)
  //
  // Therefore, we need to other events.
  CGEventMask mask = 0;
  // Do not grab NSEventTypeGesture - NSEventTypeEndGesture.
  // If we grab these events, three-finger loop up will be disabled by unknown reason.
  // (a bug of OS X?)
  for (NSEventType type = NSLeftMouseDown; type < NSEventTypeGesture; ++type) {
    if (type == NSKeyDown) continue;
    if (type == NSKeyUp) continue;

    mask |= NSEventMaskFromType(type);
  }

  // We need to specify kCGSessionEventTap to CGEventTapCreate.
  // If we pass kCGAnnotatedSessionEventTap to CGEventTapCreate,
  // media keys (brightness control, volume control, etc.) will be disabled by unknown reason.
  // (a bug of OS X?)
  eventTap = CGEventTapCreate(kCGSessionEventTap,
                              kCGHeadInsertEventTap,
                              kCGEventTapOptionDefault,
                              mask,
                              eventTapCallBack,
                              self);
  if (! eventTap) {
    NSLog(@"CGEventTapCreate is failed");
    return;
  }

  CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
  CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

  CGEventTapEnable(eventTap, 1);

  CFRelease(runLoopSource);

  // Do not release eventTap here.
  // We use eventTap in eventTapCallBack.
}

@end
