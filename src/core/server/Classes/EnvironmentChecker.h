// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface EnvironmentChecker : NSObject

+ (BOOL)checkDoubleCommand;
+ (BOOL)checkKeyRemap4MacBook;
+ (BOOL)checkKirgudu;
+ (BOOL)checkSmoothMouse;

@end
