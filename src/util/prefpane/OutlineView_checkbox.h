// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface org_pqrs_KeyRemap4MacBook_OutlineView_checkbox : NSObject
{
  IBOutlet id _outlineView_checkbox;
  IBOutlet id _searchText;
}

- (IBAction) expandALL:(id)sender;
- (IBAction) collapseALL:(id)sender;
- (IBAction) toggleShowEnabled:(id)sender;
- (IBAction) search:(id)sender;

@end
