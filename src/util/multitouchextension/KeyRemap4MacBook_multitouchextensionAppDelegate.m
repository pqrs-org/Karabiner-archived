//
//  KeyRemap4MacBook_multitouchextensionAppDelegate.m
//  KeyRemap4MacBook_multitouchextension
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#include <IOKit/IOKitLib.h>
#import "KeyRemap4MacBook_multitouchextensionAppDelegate.h"

@implementation KeyRemap4MacBook_multitouchextensionAppDelegate

- (id) init
{
  self = [super init];

  if (self) {
    mtdevices_ = [NSMutableArray new];
  }

  return self;
}

- (void) dealloc
{
  [mtdevices_ release];

  [super dealloc];
}

// ------------------------------------------------------------
struct Finger;
typedef void* MTDeviceRef;
typedef int (* MTContactCallbackFunction)(int, struct Finger*, int, double, int);

CFMutableArrayRef MTDeviceCreateList(void);
void MTRegisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
void MTUnregisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
void MTDeviceStart(MTDeviceRef, int);
void MTDeviceStop(MTDeviceRef, int);

org_pqrs_KeyRemap4MacBook_Client* global_client_ = nil;
NSMutableArray* global_mtdevices_ = nil;

static void setPreference(int newvalue) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  [[global_client_ proxy] setValueForName:newvalue forName:@"notsave.pointing_relative_to_scroll"];
  [pool drain];
}

// ------------------------------------------------------------
// Multitouch callback
static int callback(int device, struct Finger* data, int fingers, double timestamp, int frame) {
  static int current = 0;
  int newstatus = (fingers >= 3 ? 1 : 0);

  if (current != newstatus) {
    current = newstatus;
    setPreference(current);
  }

  return 0;
}

static void setcallback(BOOL isset) {
  NSArray* list = nil;
  NSEnumerator* e = nil;

  list = (NSArray*)(MTDeviceCreateList());
  if (! list) goto finish;

  e = [list objectEnumerator];
  if (! e) goto finish;

  for (;;) {
    MTDeviceRef device = [e nextObject];
    if (! device) break;

    // We need retain 'device' to prevent a mysterious crash.
    // So, we append 'device' to global_mtdevices_.
    if ([global_mtdevices_ indexOfObject:device] == NSNotFound) {
      [global_mtdevices_ addObject:device];
    }

    if (isset) {
      MTRegisterContactFrameCallback(device, callback);
      MTDeviceStart(device, 0);
    } else {
      MTUnregisterContactFrameCallback(device, callback);
      MTDeviceStop(device, 0);
    }
  }

finish:
  [list release];
}

// ------------------------------------------------------------
// Notification
static void observer_refresh(void* refcon, io_iterator_t iterator) {
  NSLog(@"[INFO] observer_refresh called\n");

  setPreference(0);

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
  IONotificationPortRef port = IONotificationPortCreate(kIOMasterPortDefault);
  if (! port) {
    NSLog(@"[ERROR] IONotificationPortCreate");
    return;
  }

  // ------------------------------------------------------------
  NSMutableDictionary* subdict = [NSMutableDictionary dictionaryWithObject:@"AppleMultitouchHIDEventDriver" forKey:@"IOClass"];
  NSMutableDictionary* match = [NSMutableDictionary dictionaryWithObject:subdict forKey:@kIOPropertyMatchKey];

  // ----------------------------------------------------------------------
  io_iterator_t it;
  kern_return_t kr;

  [match retain]; // for kIOTerminatedNotification
  kr = IOServiceAddMatchingNotification(port,
                                        kIOTerminatedNotification,
                                        (CFMutableDictionaryRef)match,
                                        &observer_refresh,
                                        NULL,
                                        &it);
  if (kr != kIOReturnSuccess) {
    NSLog(@"[ERROR] IOServiceAddMatchingNotification");
    return;
  }
  observer_refresh(NULL, it);

  [match retain]; // for kIOMatchedNotification
  kr = IOServiceAddMatchingNotification(port,
                                        kIOMatchedNotification,
                                        (CFMutableDictionaryRef)match,
                                        &observer_refresh,
                                        NULL,
                                        &it);
  if (kr != kIOReturnSuccess) {
    NSLog(@"[ERROR] IOServiceAddMatchingNotification");
    return;
  }
  observer_refresh(NULL, it);

  // ----------------------------------------------------------------------
  CFRunLoopSourceRef loopsource = IONotificationPortGetRunLoopSource(port);
  if (! loopsource) {
    NSLog(@"[ERROR] IONotificationPortGetRunLoopSource");
    return;
  }
  CFRunLoopAddSource(CFRunLoopGetCurrent(), loopsource, kCFRunLoopDefaultMode);
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  global_client_ = client_;
  global_mtdevices_ = mtdevices_;
  [self setNotification];

  // --------------------------------------------------
  NSStatusBar* statusBar = [NSStatusBar systemStatusBar];
  statusItem_ = [statusBar statusItemWithLength:24];
  [statusItem_ retain];

  [statusItem_ setTitle:@""];
  [statusItem_ setImage:[NSImage imageNamed:@"icon.statusbar.0"]];
  [statusItem_ setAlternateImage:[NSImage imageNamed:@"icon.statusbar.1"]];
  [statusItem_ setHighlightMode:YES];
  [statusItem_ setMenu:statusMenu_];
}

- (void) applicationWillTerminate:(NSNotification*)aNotification {
  setcallback(NO);

  setPreference(0);

  [statusItem_ release];
}

// ------------------------------------------------------------
- (IBAction) quit:(id)sender {
  [NSApp terminate:self];
}

@end
