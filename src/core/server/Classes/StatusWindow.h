// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import <Growl/Growl.h>
#import "PreferencesManager.h"

@interface StatusWindow : NSObject <GrowlApplicationBridgeDelegate> {
  IBOutlet PreferencesManager* preferencesmanager_;

  NSMutableArray* lines_;
  NSMutableArray* lastMessages_;
  BOOL isGrowlNotRunningWarningDisplayed_;
}

- (void) resetStatusMessage;
- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message;

@end
