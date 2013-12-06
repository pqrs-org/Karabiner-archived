// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class PreferencesManager;

@interface Updater : NSObject
{
  IBOutlet PreferencesManager* preferencesManager_;
}

- (IBAction) checkForUpdates:(id)sender;
- (IBAction) checkForUpdatesInBackground:(id)sender;

- (IBAction) checkForUpdatesStableOnly:(id)sender;
- (IBAction) checkForUpdatesWithBetaVersion:(id)sender;

@end
