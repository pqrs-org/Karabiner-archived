// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class StatusMessageView_normal;

@interface StatusWindow : NSObject {
  BOOL statusWindowPreferencesOpened_;
  NSMutableArray* lines_;
  NSMutableArray* lastMessages_;

  IBOutlet NSWindow* window_;
  IBOutlet StatusMessageView_normal* statusMessageView_normal_;
}

- (void) setupStatusWindow;

- (void) resetStatusMessage;
- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message;

- (IBAction) refresh:(id)sender;

@end
