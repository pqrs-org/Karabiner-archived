/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */
#import <Cocoa/Cocoa.h>

@interface OtherInformationStore : NSObject
{
  @private
  NSString* version_;
  NSString* applicationname_;
  NSString* inputsourcename_;

  IBOutlet id label_version_;
  IBOutlet id label_applicationname_;
  IBOutlet id label_inputsourcename_;
}

- (void) setVersion;
- (void) setApplicationName:(NSString*)name;
- (void) setInputSourceName:(NSString*)name;

- (IBAction) copy:(id)sender;

@end
