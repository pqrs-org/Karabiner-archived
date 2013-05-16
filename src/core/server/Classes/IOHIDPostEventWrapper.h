// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface IOHIDPostEventWrapper : NSObject {
  mach_port_t eventDriver_;
}

- (void) postKey:(uint8_t)keyCode;

@end
