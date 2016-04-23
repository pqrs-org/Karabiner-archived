/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@import Cocoa;

@interface ServerController : NSObject

- (void)terminateServerProcess;
- (void)updateStartAtLogin:(BOOL)preferredValue;

@end
