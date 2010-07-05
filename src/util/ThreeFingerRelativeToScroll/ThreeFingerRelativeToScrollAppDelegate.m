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
  snprintf(buf, sizeof(buf), "/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set notsave.pointing_relative_to_scroll %d", newvalue);
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
static void observer_refresh(void* refcon, io_iterator_t iterator) {
  NSLog(@"[INFO] observer_refresh called\n");

  sysctl_set(0);

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
  [subdict setObject:@"AppleMultitouchHIDEventDriver" forKey:@"IOClass"];
  [match setObject:subdict forKey:@kIOPropertyMatchKey];
  [match retain]; // for kIOTerminatedNotification
  [match retain]; // for kIOMatchedNotification

  IONotificationPortRef port = IONotificationPortCreate(kIOMasterPortDefault);
  if (! port) {
    NSLog(@"[ERROR] IONotificationPortCreate");
    return;
  }

  // ----------------------------------------------------------------------
  io_iterator_t it;
  kern_return_t kr;

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

// ------------------------------------------------------------
- (NSURL*) appURL {
  return [NSURL fileURLWithPath:[[NSBundle mainBundle] bundlePath]];
}

- (LSSharedFileListItemRef) getLSSharedFileListItemRef:(LSSharedFileListRef)loginItems {
  if (! loginItems) return NULL;

  LSSharedFileListItemRef retval = NULL;
  NSURL* appURL = [self appURL];

  UInt32 seed = 0U;
  NSArray* currentLoginItems = [NSMakeCollectable (LSSharedFileListCopySnapshot(loginItems, &seed))autorelease];
  for (id itemObject in currentLoginItems) {
    LSSharedFileListItemRef item = (LSSharedFileListItemRef)itemObject;

    UInt32 resolutionFlags = kLSSharedFileListNoUserInteraction | kLSSharedFileListDoNotMountVolumes;
    CFURLRef url = NULL;
    OSStatus err = LSSharedFileListItemResolve(item, resolutionFlags, &url, NULL);
    if (err == noErr) {
      BOOL foundIt = CFEqual(url, appURL);
      CFRelease(url);

      if (foundIt) {
        retval = item;
        break;
      }
    }
  }

  return retval;
}

- (BOOL) isStartAtLogin {
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (! loginItems) return NO;

  LSSharedFileListItemRef item = [self getLSSharedFileListItemRef:loginItems];
  CFRelease(loginItems);

  return item != NULL;
}

- (void) enableStartAtLogin {
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (! loginItems) return;

  NSURL* appURL = [self appURL];
  LSSharedFileListItemRef item = LSSharedFileListInsertItemURL(loginItems, kLSSharedFileListItemLast, NULL, NULL, (CFURLRef)(appURL), NULL, NULL);
  if (item) {
    CFRelease(item);
  }
  CFRelease(loginItems);
}

- (void) disableStartAtLogin {
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (! loginItems) return;

  LSSharedFileListItemRef item = [self getLSSharedFileListItemRef:loginItems];
  if (item) {
    LSSharedFileListItemRemove(loginItems, item);
  }
  CFRelease(loginItems);
}

// ------------------------------------------------------------
- (IBAction) setStartAtLogin:(id)sender {
  if ([self isStartAtLogin]) {
    [self disableStartAtLogin];
    [sender setState:NSOffState];
  } else {
    [self enableStartAtLogin];
    [sender setState:NSOnState];
  }
}

- (void) menuNeedsUpdate:(NSMenu*)menu {
  if ([self isStartAtLogin]) {
    [_startAtLoginMenuItem setState:NSOnState];
  } else {
    [_startAtLoginMenuItem setState:NSOffState];
  }
}

@end
