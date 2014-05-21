/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */
#import <Cocoa/Cocoa.h>

@interface AppQueue : NSObject
{
  @private
  NSMutableArray* queue_;
  IBOutlet id view_;
}

- (void) push:(NSDictionary*)dictionary;

- (IBAction) clear:(id)sender;
- (IBAction) copy:(id)sender;

@end
