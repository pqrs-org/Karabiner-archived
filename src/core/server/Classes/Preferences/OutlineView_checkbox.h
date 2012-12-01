// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "OutlineView.h"

@interface OutlineView_checkbox : OutlineView
{
  IBOutlet NSSearchField* searchText_;
  IBOutlet NSButton* showEnabledOnly_;
}

- (IBAction) reloadXML:(id)sender;
- (IBAction) filter:(id)sender;

@end
