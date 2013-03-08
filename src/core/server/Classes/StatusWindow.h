// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class StatusMessageView_normal;

@interface StatusWindow : NSObject {
  BOOL statusWindowPreferencesOpened_;
  NSMutableArray* windows_;
  NSMutableArray* lines_;
  NSMutableArray* lastMessages_;

  IBOutlet NSWindow* statusMessage_normal_;
  IBOutlet NSWindow* statusMessage_nano_;
  IBOutlet NSWindow* statusMessage_edge_;
}

- (void) setupStatusWindow;

- (void) resetStatusMessage;
- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message;

- (IBAction) refresh:(id)sender;

@end
