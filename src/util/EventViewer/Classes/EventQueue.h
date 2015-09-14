/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import <Cocoa/Cocoa.h>

@class KarabinerClient;

@interface EventQueue : NSObject {
@private
  NSMutableArray* queue_;
  IBOutlet NSTableView* view_;
  IBOutlet KarabinerClient* client_;
}

- (void)pushFromNSApplication:(NSEvent*)event;
- (void)pushMouseEvent:(NSEvent*)event;

- (IBAction)clear:(id)sender;
- (IBAction)copy:(id)sender;

@end
