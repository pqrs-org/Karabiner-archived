// -*- Mode: objc -*-

@import Cocoa;

@interface StatusMessageView : NSView

@property(weak) IBOutlet NSTextField* message;

- (void)updateMessage:(NSString*)message;
- (void)updateWindowFrame:(NSScreen*)screen;

@end
