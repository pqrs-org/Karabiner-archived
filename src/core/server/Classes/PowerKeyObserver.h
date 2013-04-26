// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class ClientForKernelspace;

@interface PowerKeyObserver : NSObject {
  CFMachPortRef eventTap;
  BOOL enqueued;
  BOOL shouldBlockPowerKeyKeyCode;

  IBOutlet ClientForKernelspace* clientForKernelspace;
}

@property (assign) CFMachPortRef eventTap;
@property (assign) BOOL enqueued;
@property (assign) BOOL shouldBlockPowerKeyKeyCode;
@property (assign) ClientForKernelspace* clientForKernelspace;

- (void) start;

@end
