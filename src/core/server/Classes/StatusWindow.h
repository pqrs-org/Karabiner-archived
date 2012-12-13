// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class StatusMessageView;

@interface StatusWindow : NSObject {
  BOOL statusWindowPreferencesOpened_;
  NSMutableArray* lines_;
  NSMutableArray* lastMessages_;

  IBOutlet NSWindow* window_;
  IBOutlet StatusMessageView* statusMessageView_;
}

- (void) setupStatusWindow;

- (void) resetStatusMessage;
- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message;

- (IBAction) refresh:(id)sender;

@end
