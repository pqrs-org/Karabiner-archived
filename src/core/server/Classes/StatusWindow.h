// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesManager.h"

@interface StatusWindow : NSObject {
  IBOutlet NSWindow* statuswindow_;
  IBOutlet NSTextField* label_;
  IBOutlet NSImageView* backgroud_;
  IBOutlet PreferencesManager* preferencesmanager_;

  NSMutableArray* lines_;
}

- (void) setupStatusWindow;

- (void) resetStatusMessage;
- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message;

- (void) refreshWindowPosition;

@end
