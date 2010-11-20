// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesClient.h"

@interface org_pqrs_KeyRemap4MacBook_OutlineView_checkbox : NSObject
{
  NSArray* datasource_;

  IBOutlet id outlineview_checkbox_;
  IBOutlet id searchText_;
  IBOutlet id showEnabledOnly_;
  IBOutlet org_pqrs_KeyRemap4MacBook_PreferencesClient* preferencesclient_;
}

- (IBAction) reloadXML:(id)sender;
- (IBAction) filter:(id)sender;

@end
