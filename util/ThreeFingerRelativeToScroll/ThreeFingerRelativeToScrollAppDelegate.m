//
//  ThreeFingerRelativeToScrollAppDelegate.m
//  ThreeFingerRelativeToScroll
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ThreeFingerRelativeToScrollAppDelegate.h"
#include <IOKit/IOKitLib.h>

@implementation ThreeFingerRelativeToScrollAppDelegate

// ------------------------------------------------------------
struct Finger;
typedef void* MTDeviceRef;
typedef int (* MTContactCallbackFunction)(int, struct Finger*, int, double, int);

CFMutableArrayRef MTDeviceCreateList(void);
void MTRegisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
void MTUnregisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
void MTDeviceStart(MTDeviceRef, int);
void MTDeviceStop(MTDeviceRef, int);

// ------------------------------------------------------------
// Multitouch callback
static void sysctl_set(int newvalue) {
  char buf[512];
  snprintf(buf, sizeof(buf), "/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set remap.pointing_relative_to_scroll %d", newvalue);
  system(buf);
}

static int callback(int device, struct Finger* data, int fingers, double timestamp, int frame) {
  static int current = 0;
  int newstatus = (fingers >= 3 ? 1 : 0);

  if (current != newstatus) {
    current = newstatus;
    sysctl_set(current);
  }

  return 0;
}

static void setcallback(BOOL isset) {
  NSMutableArray* list = (NSMutableArray*)MTDeviceCreateList();
  if (! list) return;

  NSEnumerator* e = [list objectEnumerator];
  if (! e) return;

  for (;;) {
    MTDeviceRef device = [e nextObject];
    if (! device) break;

    if (isset) {
      MTRegisterContactFrameCallback(device, callback);
      MTDeviceStart(device, 0);
    } else {
      MTUnregisterContactFrameCallback(device, callback);
      MTDeviceStop(device, 0);
    }
  }
}

// ------------------------------------------------------------
// Notification
static void observer_kIOMatchedNotification(void* refcon, io_iterator_t iterator) {
  bool isfound = false;
  while (IOIteratorNext(iterator)) {
    isfound = true;
  }
  if (isfound) {
    // wait for the initialization of the device
    sleep(1);

    setcallback(NO);
    setcallback(YES);
  }
}

- (void) setNotification {
  NSMutableDictionary* match = [NSMutableDictionary dictionary];
  NSMutableDictionary* subdict = [NSMutableDictionary dictionary];
  [subdict setObject:@"AppleMultitouchDevice" forKey:@"IOClass"];
  [match setObject:subdict forKey:@kIOPropertyMatchKey];
  [match retain];

  IONotificationPortRef port = IONotificationPortCreate(kIOMasterPortDefault);
  if (! port) {
    NSLog(@"[ERROR] IONotificationPortCreate");
    return;
  }

  io_iterator_t it;
  kern_return_t kr = IOServiceAddMatchingNotification(port,
                                                      kIOMatchedNotification,
                                                      (CFMutableDictionaryRef)match,
                                                      &observer_kIOMatchedNotification,
                                                      NULL,
                                                      &it);
  if (kr != kIOReturnSuccess) {
    NSLog(@"[ERROR] IOServiceAddMatchingNotification");
    return;
  }
  observer_kIOMatchedNotification(NULL, it);

  CFRunLoopSourceRef loopsource = IONotificationPortGetRunLoopSource(port);
  if (! loopsource) {
    NSLog(@"[ERROR] IONotificationPortGetRunLoopSource");
    return;
  }
  CFRunLoopAddSource(CFRunLoopGetCurrent(), loopsource, kCFRunLoopDefaultMode);
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [self setNotification];

  // --------------------------------------------------
  NSStatusBar* statusBar = [NSStatusBar systemStatusBar];
  _statusItem = [statusBar statusItemWithLength:24];
  [_statusItem retain];

  [_statusItem setTitle:@""];
  [_statusItem setImage:[NSImage imageNamed:@"icon.statusbar.0"]];
  [_statusItem setAlternateImage:[NSImage imageNamed:@"icon.statusbar.1"]];
  [_statusItem setHighlightMode:YES];
  [_statusItem setMenu:_statusMenu];
}

- (void) applicationWillTerminate:(NSNotification*)aNotification {
  setcallback(NO);

  sysctl_set(0);

  [_statusItem release];
}

// ------------------------------------------------------------
- (IBAction) quit:(id)sender {
  [NSApp terminate:self];
}

@end
