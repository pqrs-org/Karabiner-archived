// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface SessionObserver : NSObject

- (instancetype)init:(NSTimeInterval)seconds
              active:(void (^)(void))active
            inactive:(void (^)(void))inactive;

@end
