// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface EnvironmentChecker : NSObject

+ (BOOL)checkDoubleCommand;
+ (BOOL)checkKeyRemap4MacBook;
+ (BOOL)checkKirgudu;
+ (BOOL)checkSmoothMouse;

@end
