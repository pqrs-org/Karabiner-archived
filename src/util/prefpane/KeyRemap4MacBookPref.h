// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <PreferencePanes/PreferencePanes.h>

@interface KeyRemap4MacBookPref : NSPreferencePane
{
  IBOutlet id _versionText;
  IBOutlet id _checkbox_statusbar;
}

- (IBAction) toggleStatusBar:(id)sender;
- (IBAction) uninstall:(id)sender;
- (void) mainViewDidLoad;

@end
