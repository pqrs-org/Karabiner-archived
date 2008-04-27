// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface TableView_list : NSObject
{
  IBOutlet id _tableView;
}

- (IBAction) add:(id)sender;
- (IBAction) delete:(id)sender;
- (IBAction) refresh:(id)sender;

@end
