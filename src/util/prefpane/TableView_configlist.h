// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesClient.h"

@interface TableView_configlist : NSObject
{
  @private
  IBOutlet id view_;
  IBOutlet KeyRemap4MacBookPreferencesClient* preferencesclient_;
}

- (IBAction) add:(id)sender;
- (IBAction) delete:(id)sender;

@end
