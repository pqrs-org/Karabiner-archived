// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <PreferencePanes/PreferencePanes.h>

@interface org_pqrs_KeyRemap4MacBook_KeyRemap4MacBookPref : NSPreferencePane
{
  IBOutlet id _versionText;
  IBOutlet id _checkbox_statusbar;
}

- (IBAction) toggleStatusBar:(id)sender;
- (void) mainViewDidLoad;

@end
