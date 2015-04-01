// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface StatusMessageManager : NSObject {
  IBOutlet NSWindow* statusMessage_normal_;
  IBOutlet NSWindow* statusMessage_nano_;
  IBOutlet NSWindow* statusMessage_edge_;
}

- (void)setupStatusMessageManager;

- (void)resetStatusMessage;
- (void)setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message;

- (IBAction)refresh:(id)sender;

@end
