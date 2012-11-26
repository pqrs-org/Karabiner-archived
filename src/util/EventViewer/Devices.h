/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface Devices : NSObject
{
  @private
  NSMutableArray* devices_;
  IBOutlet id view_;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;
}

- (IBAction) refresh:(id)sender;
- (IBAction) copy:(id)sender;

@end
