// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <PreferencePanes/PreferencePanes.h>
#import "OutlineView_checkbox.h"
#import "OutlineView_number.h"

@interface org_pqrs_KeyRemap4MacBookPref : NSPreferencePane
{
  IBOutlet id _versionText;
  IBOutlet id _checkbox_statusbar;
}

- (IBAction) toggleStatusBar:(id)sender;
- (void) mainViewDidLoad;

@end
