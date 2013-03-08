// -*- Mode: objc -*-

@interface StatusMessageView : NSView
{
  IBOutlet NSTextField* message_;
}

- (void) setMessage:(NSString*)message;
- (void) updateWindowFrame;

@end
