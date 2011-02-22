//
//  KeyRemap4MacBook_multitouchextensionAppDelegate.m
//  KeyRemap4MacBook_multitouchextension
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#include <IOKit/IOKitLib.h>
#import "KeyRemap4MacBook_multitouchextensionAppDelegate.h"

enum { MAX_FINGERS = 3 };
static int current_status_[MAX_FINGERS];
static BOOL isSessionActive_ = NO;

@implementation KeyRemap4MacBook_multitouchextensionAppDelegate

- (id) init
{
  self = [super init];

  if (self) {
    mtdevices_ = [NSMutableArray new];
    for (int i = 0; i < MAX_FINGERS; ++i) {
      current_status_[i] = 0;
    }
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

static void setPreference(int fingers, int newvalue) {
  // We observe NSWorkspaceSessionDidResignActiveNotification and call [setcallback:NO] in callback function.
  // But it does not work well!!!
  // In inactive session, the callback function which is registered by MTRegisterContactFrameCallback is still called.
  //
  // So, we need to check session status by ourself.
  // The isSessionActive_ variable synchronizes with session status of NSWorkspace.

  if (isSessionActive_) {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    {
      NSString* name = [PreferencesController getSettingName:fingers];
      if ([name length] > 0) {
        [[global_client_ proxy] setValueForName:newvalue forName:name];
      }
    }
    [pool drain];
  }
}

- (void) resetPreferences
{
  for (int i = 0; i < MAX_FINGERS; ++i) {
    setPreference(i + 1, 0);
  }
}

// ------------------------------------------------------------
// Multitouch callback
static int callback(int device, struct Finger* data, int fingers, double timestamp, int frame) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  {
    // deactivate settings first.
    for (int i = 0; i < MAX_FINGERS; ++i) {
      if (current_status_[i] && fingers != i + 1) {
        current_status_[i] = 0;
        setPreference(i + 1, 0);
      }
    }

    if (fingers > 0 && current_status_[fingers - 1] == 0) {
      current_status_[fingers - 1] = 1;

      if ([PreferencesController isSettingEnabled:fingers]) {
        setPreference(fingers, 1);
      }
    }
  }
  [pool drain];

  return 0;
}

- (void) setcallback:(BOOL)isset {
  @synchronized(self) {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    {
      NSArray* list = nil;

      list = (NSArray*)(MTDeviceCreateList());
      if (! list) goto finish;

      for (NSUInteger i = 0; i < [list count]; ++i) {
        MTDeviceRef device = [list objectAtIndex:i];
        if (! device) continue;

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

      [self resetPreferences];
    }
    [pool drain];
  }
}

// ------------------------------------------------------------
// Notification
- (void) release_iterator:(io_iterator_t)iterator {
  for (;;) {
    io_object_t obj = IOIteratorNext(iterator);
    if (! obj) break;

    IOObjectRelease(obj);
  }
}

static void observer_refresh(void* refcon, io_iterator_t iterator) {
  NSLog(@"[INFO] observer_refresh called\n");

  KeyRemap4MacBook_multitouchextensionAppDelegate* self = refcon;

  [self release_iterator:iterator];

  // wait for the initialization of the device
  sleep(1);

  [self setcallback:NO];
  [self setcallback:YES];
}

- (void) setNotification {
  IONotificationPortRef port = IONotificationPortCreate(kIOMasterPortDefault);
  if (! port) {
    NSLog(@"[ERROR] IONotificationPortCreate");
    return;
  }

  // ------------------------------------------------------------
  NSMutableDictionary* match = [(NSMutableDictionary*)(IOServiceMatching("AppleMultitouchDevice")) autorelease];

  // ----------------------------------------------------------------------
  io_iterator_t it;
  kern_return_t kr;

  [match retain]; // for kIOTerminatedNotification
  kr = IOServiceAddMatchingNotification(port,
                                        kIOTerminatedNotification,
                                        (CFMutableDictionaryRef)match,
                                        &observer_refresh,
                                        self,
                                        &it);
  if (kr != kIOReturnSuccess) {
    NSLog(@"[ERROR] IOServiceAddMatchingNotification");
    return;
  }
  [self release_iterator:it];

  [match retain]; // for kIOMatchedNotification
  kr = IOServiceAddMatchingNotification(port,
                                        kIOMatchedNotification,
                                        (CFMutableDictionaryRef)match,
                                        &observer_refresh,
                                        self,
                                        &it);
  if (kr != kIOReturnSuccess) {
    NSLog(@"[ERROR] IOServiceAddMatchingNotification");
    return;
  }
  [self release_iterator:it];

  // ----------------------------------------------------------------------
  CFRunLoopSourceRef loopsource = IONotificationPortGetRunLoopSource(port);
  if (! loopsource) {
    NSLog(@"[ERROR] IONotificationPortGetRunLoopSource");
    return;
  }
  CFRunLoopAddSource(CFRunLoopGetCurrent(), loopsource, kCFRunLoopDefaultMode);
}

// ------------------------------------------------------------
- (void) observer_NSWorkspaceSessionDidBecomeActiveNotification:(NSNotification*)notification
{
  NSLog(@"observer_NSWorkspaceSessionDidBecomeActiveNotification");
  isSessionActive_ = YES;
  [self setcallback:YES];
}

- (void) observer_NSWorkspaceSessionDidResignActiveNotification:(NSNotification*)notification
{
  NSLog(@"observer_NSWorkspaceSessionDidResignActiveNotification");
  isSessionActive_ = NO;
  [self setcallback:NO];
}

- (void) observer_NSWorkspaceDidWakeNotification:(NSNotification*)notification
{
  NSLog(@"observer_NSWorkspaceDidWakeNotification");
  [self setcallback:YES];
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  isSessionActive_ = YES;

  [preferences_ load];

  global_client_ = client_;
  global_mtdevices_ = mtdevices_;

  [self setNotification];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceSessionDidBecomeActiveNotification:)
                                                             name:NSWorkspaceSessionDidBecomeActiveNotification
                                                           object:nil];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceSessionDidResignActiveNotification:)
                                                             name:NSWorkspaceSessionDidResignActiveNotification
                                                           object:nil];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidWakeNotification:)
                                                             name:NSWorkspaceDidWakeNotification
                                                           object:nil];

  [self setcallback:YES];
}

- (void) applicationWillTerminate:(NSNotification*)aNotification {
  [self setcallback:NO];
}

- (BOOL) applicationShouldHandleReopen:(NSApplication*)theApplication hasVisibleWindows:(BOOL)flag
{
  [preferences_ show];
  return YES;
}

@end
