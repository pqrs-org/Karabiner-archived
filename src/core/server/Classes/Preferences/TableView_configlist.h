// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class PreferencesManager;

@interface TableView_configlist : NSObject
{
  @private
  IBOutlet NSTableView* view_;
  IBOutlet PreferencesManager* preferencesManager_;
}

- (IBAction) add:(id)sender;
- (IBAction) delete:(id)sender;

@end
