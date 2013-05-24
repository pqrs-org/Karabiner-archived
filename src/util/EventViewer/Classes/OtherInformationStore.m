/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OtherInformationStore.h"

@implementation OtherInformationStore

- (IBAction) copy:(id)sender
{
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [[NSMutableString new] autorelease];

  [string appendFormat:@"Language Code: %@\n", languagecode_];
  [string appendFormat:@"Input Source ID: %@\n", inputsourceid_];
  [string appendFormat:@"Input Mode ID: %@\n", inputmodeid_];

  [pboard clearContents];
  [pboard writeObjects:[NSArray arrayWithObject:string]];
}

- (void) setLanguageCode:(NSString*)value
{
  if (! value) {
    value = @"---";
  }

  languagecode_ = value;
  [label_languagecode_ setStringValue:value];
}

- (void) setInputSourceID:(NSString*)value
{
  if (! value) {
    value = @"---";
  }

  inputsourceid_ = value;
  [label_inputsourceid_ setStringValue:value];
}

- (void) setInputModeID:(NSString*)value
{
  if (! value) {
    value = @"---";
  }

  inputmodeid_ = value;
  [label_inputmodeid_ setStringValue:value];
}

@end
