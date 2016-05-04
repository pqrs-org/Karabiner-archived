/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@import Cocoa;

@class ServerObjects;

@interface PreferencesWindowController : NSWindowController <NSWindowDelegate, NSTabViewDelegate>

@property(weak) ServerObjects* serverObjects;

- (void)setup;
- (void)show;
- (void)savePreferencesModel;

@end
