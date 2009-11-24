//
//  ThreeFingerRelativeToScrollAppDelegate.m
//  ThreeFingerRelativeToScroll
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ThreeFingerRelativeToScrollAppDelegate.h"

@implementation ThreeFingerRelativeToScrollAppDelegate

@synthesize window;

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
static int callback(int device, struct Finger *data, int fingers, double timestamp, int frame) {
  static int current = 0;

  {
    int newstatus = (fingers >= 3 ? 1 : 0);
    if (current == newstatus) return 0;
    current = newstatus;
  }

  {
    char buf[512];
    snprintf(buf, sizeof(buf), "/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set remap.pointing_relative_to_scroll %d", current);
    system(buf);
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
}

@end
