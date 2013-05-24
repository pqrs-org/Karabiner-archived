/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface Devices : NSObject
{
  @private
  NSMutableArray* devices_;
  IBOutlet id view_;
  IBOutlet KeyRemap4MacBookClient* client_;
}

- (IBAction) refresh:(id)sender;
- (IBAction) copy:(id)sender;

@end
