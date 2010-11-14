// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesManager.h"

@interface StatusBar : NSObject {
  NSStatusItem* statusItem_;
  IBOutlet NSMenu* menu_;
  IBOutlet PreferencesManager* preferencesmanager_;
}

- (void) refresh;
- (void) statusBarItemSelected:(id)sender;

- (IBAction) openPreferencePane:(id)sender;
- (IBAction) launchEventViewer:(id)sender;

@end
