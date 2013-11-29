// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface Relauncher : NSObject

+ (void) resetRelaunchedCount;
+ (void) relaunch;
+ (BOOL) isEqualPreviousProcessVersionAndCurrentProcessVersion;

@end
