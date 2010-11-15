// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesClient.h"

@interface org_pqrs_KeyRemap4MacBook_OutlineView_checkbox : NSObject
{
  IBOutlet id _outlineView_checkbox;
  IBOutlet id _searchText;
  IBOutlet id _showEnabledOnly;
  IBOutlet KeyRemap4MacBookPreferencesClient* preferencesclient_;
}

- (IBAction) expandALL:(id)sender;
- (IBAction) collapseALL:(id)sender;
- (IBAction) filter:(id)sender;

@end
