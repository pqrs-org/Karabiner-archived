// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class PreferencesManager;

@interface Updater : NSObject

- (void)checkForUpdatesInBackground;
- (void)checkForUpdatesStableOnly;
- (void)checkForUpdatesWithBetaVersion;

@end
