// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesClient.h"

@interface org_pqrs_KeyRemap4MacBook_OutlineView : NSObject {
  NSArray* datasource_;
  BOOL ischeckbox_;

  IBOutlet id outlineview_;
  IBOutlet org_pqrs_KeyRemap4MacBook_PreferencesClient* preferencesclient_;
}

- (void) load:(BOOL)force;

@end
