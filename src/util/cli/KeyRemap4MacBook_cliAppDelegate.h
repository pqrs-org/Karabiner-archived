// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesClient.h"

@interface KeyRemap4MacBook_cliAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet org_pqrs_KeyRemap4MacBook_PreferencesClient* preferencesclient_;
}

@property (assign) IBOutlet NSWindow* window;

@end
