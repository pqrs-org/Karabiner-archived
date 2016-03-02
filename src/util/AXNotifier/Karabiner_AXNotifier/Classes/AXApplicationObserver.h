// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface AXApplicationObserver : NSObject

@property(readonly) NSRunningApplication* runningApplication;

- (instancetype)initWithRunningApplication:(NSRunningApplication*)runningApplication preferences:(NSDictionary*)preferences;

// You have to observe kAXTitleChangedNotification by hand because
// you generally want to observe the notification only in the front application.
- (void)unobserveTitleChangedNotification;
- (void)observeTitleChangedNotification;

- (void)postNotification;

@end
