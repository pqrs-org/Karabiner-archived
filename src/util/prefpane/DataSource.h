// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesClient.h"

@interface org_pqrs_KeyRemap4MacBook_DataSource : NSObject {
  SEL selector_;
  IBOutlet org_pqrs_KeyRemap4MacBook_PreferencesClient* preferencesclient_;
  NSArray* datasource_;
}

- initWithSelector:(SEL)sel preferencesclient:(org_pqrs_KeyRemap4MacBook_PreferencesClient*)client;
- (void) load:(BOOL)force;

@end
