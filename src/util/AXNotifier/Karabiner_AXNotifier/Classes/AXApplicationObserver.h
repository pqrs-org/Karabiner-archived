// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface AXApplicationObserver : NSObject

- (instancetype) initWithRunningApplication:(NSRunningApplication*)runningApplication;

// You have to observe kAXTitleChangedNotification by hand because
// you generally want to observe the notification only in the front application.
- (void) unobserveTitleChangedNotification;
- (void) observeTitleChangedNotification;

- (void) postNotification;

@property NSRunningApplication* runningApplication;

@end
