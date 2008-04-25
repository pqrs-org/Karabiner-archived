// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <PreferencePanes/PreferencePanes.h>
#import "OutlineView_checkbox.h"
#import "OutlineView_number.h"

@interface KeyRemap4MacBookPref : NSPreferencePane
{
  IBOutlet id _versionText;
}

- (void) mainViewDidLoad;

@end
