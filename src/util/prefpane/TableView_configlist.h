// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface TableView_configlist : NSObject
{
  @private
  NSMutableArray* cache_;
  IBOutlet id view_;
}

- (void) refresh;

- (IBAction) add:(id)sender;
- (IBAction) delete:(id)sender;

@end
