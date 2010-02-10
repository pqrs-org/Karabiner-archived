/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */
#import <Cocoa/Cocoa.h>

@interface EventQueue : NSObject
{
  @private
  NSMutableArray* queue_;
  IBOutlet id view_;

  NSString* applicationname_;
  NSString* inputsourcename_;

  IBOutlet id label_applicationname_;
  IBOutlet id label_inputsourcename_;
}

- (void) push:(NSString*)eventType code:(NSString*)code name:(NSString*)name flags:(NSString*)flags misc:(NSString*)misc;
- (void) setApplicationName:(NSString*)name;
- (void) setInputSourceName:(NSString*)name;

- (IBAction) clear:(id)sender;
- (IBAction) copy:(id)sender;

@end
