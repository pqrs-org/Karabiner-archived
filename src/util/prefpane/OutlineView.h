// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface org_pqrs_KeyRemap4MacBook_OutlineView : NSObject {
  NSArray* datasource_;
  NSString* error_message_;
  BOOL ischeckbox_;

  IBOutlet id outlineview_;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;
}

- (void) load:(BOOL)force;
- (void) filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string;

@end
