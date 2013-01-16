// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface IOHIDPostEventWrapper : NSObject {
  mach_port_t eventDriver_;
}

- (void) postAuxKey:(uint8_t)auxKeyCode;

@end
