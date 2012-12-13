// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "PreferencesManager.h"

@class StatusMessageView;

@interface StatusWindow : NSObject {
  NSMutableArray* lines_;
  NSMutableArray* lastMessages_;
  BOOL isGrowlNotRunningWarningDisplayed_;

  IBOutlet NSWindow* window_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet StatusMessageView* statusMessageView_;
}

- (void) setupStatusWindow;

- (void) resetStatusMessage;
- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message;

@end
