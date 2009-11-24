//
//  ThreeFingerRelativeToScrollAppDelegate.m
//  ThreeFingerRelativeToScroll
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ThreeFingerRelativeToScrollAppDelegate.h"

@implementation ThreeFingerRelativeToScrollAppDelegate

// ------------------------------------------------------------
struct Finger;
typedef void *MTDeviceRef;
typedef int (*MTContactCallbackFunction)(int,struct Finger*,int,double,int);

CFMutableArrayRef MTDeviceCreateList(void);
void MTRegisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
void MTUnregisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
void MTDeviceStart(MTDeviceRef, int);
void MTDeviceStop(MTDeviceRef, int);

// ------------------------------------------------------------
static void
sysctl_set(int newvalue)
{
  char buf[512];
  snprintf(buf, sizeof(buf), "/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set remap.pointing_relative_to_scroll %d", newvalue);
  system(buf);
}

static int callback(int device, struct Finger *data, int fingers, double timestamp, int frame) {
  static int current = 0;
  int newstatus = (fingers >= 3 ? 1 : 0);

  if (current != newstatus) {
    current = newstatus;
    sysctl_set(current);
  }

  return 0;
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  NSMutableArray* list = (NSMutableArray*)MTDeviceCreateList();
  NSEnumerator *e = [list objectEnumerator];
  for (;;) {
    MTDeviceRef device = [e nextObject];
    if (! device) break;

    MTRegisterContactFrameCallback(device, callback);
    MTDeviceStart(device, 0);
  }

  // --------------------------------------------------
  NSStatusBar *statusBar = [NSStatusBar systemStatusBar];
  _statusItem = [statusBar statusItemWithLength:24];
  [_statusItem retain];

  [_statusItem setTitle:@""];
  [_statusItem setImage:[NSImage imageNamed:@"icon.statusbar.0"]];
  [_statusItem setAlternateImage:[NSImage imageNamed:@"icon.statusbar.1"]];
  [_statusItem setHighlightMode:YES];
  [_statusItem setMenu:_statusMenu];
}

- (void) applicationWillTerminate:(NSNotification*)aNotification {
  NSMutableArray* list = (NSMutableArray*)MTDeviceCreateList();
  NSEnumerator *e = [list objectEnumerator];
  for (;;) {
    MTDeviceRef device = [e nextObject];
    if (! device) break;
    MTUnregisterContactFrameCallback(device, callback);
    MTDeviceStop(device, 0);
  }

  // --------------------------------------------------
  sysctl_set(0);
}

// ------------------------------------------------------------
- (IBAction) quit:(id)sender {
  [NSApp terminate:self];
}

@end
