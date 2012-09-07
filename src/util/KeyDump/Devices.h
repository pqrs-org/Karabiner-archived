/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */
#import <Cocoa/Cocoa.h>

@interface Devices : NSObject
{
  @private
  NSDictionary* devices_;
  IBOutlet id view_;
  IBOutlet id segment_;
}

- (IBAction) refresh:(id)sender;
- (IBAction) copy:(id)sender;

@end
