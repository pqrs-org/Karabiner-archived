/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */
#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface OtherInformationStore : NSObject
{
  @private
  NSString* version_;
  NSString* applicationbundleidentifier_;
  NSString* languagecode_;
  NSString* inputsourceid_;
  NSString* inputmodeid_;

  IBOutlet id label_version_;
  IBOutlet id label_applicationbundleidentifier_;
  IBOutlet id label_languagecode_;
  IBOutlet id label_inputsourceid_;
  IBOutlet id label_inputmodeid_;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;
}

- (void) setVersion;
- (void) setApplicationBundleIdentifier:(NSString*)value;
- (void) setLanguageCode:(NSString*)value;
- (void) setInputSourceID:(NSString*)value;
- (void) setInputModeID:(NSString*)value;

- (IBAction) copy:(id)sender;

@end
