// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class PreferencesManager;

@interface StatusBar : NSObject {
  IBOutlet NSMenu* menu_;
  IBOutlet PreferencesManager* preferencesManager_;
}

- (void) refresh;
- (void) statusBarItemSelected:(id)sender;

@end
