// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class ClientForKernelspace;

@interface PowerKeyObserver : NSObject {
  CFMachPortRef eventTap;
  BOOL enqueued;
  BOOL shouldBlockPowerKeyKeyCode;
  CGEventRef savedPowerButtonEvent;
  BOOL ignoreNext_POWER_KEY_TYPE_SUBTYPE;

  IBOutlet ClientForKernelspace* clientForKernelspace;
}

@property (assign) CFMachPortRef eventTap;
@property (assign) BOOL enqueued;
@property (assign) BOOL shouldBlockPowerKeyKeyCode;
@property (assign) ClientForKernelspace* clientForKernelspace;
@property (assign) CGEventRef savedPowerButtonEvent;
@property (assign) BOOL ignoreNext_POWER_KEY_TYPE_SUBTYPE;

- (void) start;

@end
