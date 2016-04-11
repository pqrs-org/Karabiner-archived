// -*- Mode: objc -*-

@import Cocoa;

@interface FingerStatus : NSObject

- (instancetype)init;
- (void)clear;
- (void)add:(int)identifier active:(BOOL)active;
- (BOOL)isActive:(int)identifier;

@end
