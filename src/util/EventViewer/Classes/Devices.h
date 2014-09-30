/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import <Cocoa/Cocoa.h>
#import "KarabinerClient.h"

@interface Devices : NSObject {
@private
  NSMutableArray* devices_;
  IBOutlet id view_;
  IBOutlet KarabinerClient* client_;
}

- (IBAction)refresh:(id)sender;
- (IBAction)copy:(id)sender;

@end
