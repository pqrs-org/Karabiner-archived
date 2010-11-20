// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "OutlineView.h"

@interface org_pqrs_KeyRemap4MacBook_OutlineView_checkbox : org_pqrs_KeyRemap4MacBook_OutlineView
{
  IBOutlet id searchText_;
  IBOutlet id showEnabledOnly_;
}

- (IBAction) reloadXML:(id)sender;
- (IBAction) filter:(id)sender;

@end
