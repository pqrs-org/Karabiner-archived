// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface IOHIDPostEventWrapper : NSObject

// mask:
// * NX_ALPHASHIFTMASK
// * NX_SHIFTMASK
// * NX_CONTROLMASK
// * NX_ALTERNATEMASK
// * NX_COMMANDMASK
- (void)postModifierKey:(IOOptionBits)mask keydown:(BOOL)keydown;

- (void)postKey:(uint8_t)keyCode;

@end
