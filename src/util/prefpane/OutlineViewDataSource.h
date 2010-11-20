// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesClient.h"

@interface org_pqrs_KeyRemap4MacBook_OutlineViewDataSource : NSObject {
  SEL selector_;
  NSArray* datasource_;

  IBOutlet org_pqrs_KeyRemap4MacBook_PreferencesClient* preferencesclient_;
}

- (void) load:(BOOL)force;

@end
