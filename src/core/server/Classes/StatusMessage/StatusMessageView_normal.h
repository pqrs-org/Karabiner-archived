// -*- Mode: objc -*-

@interface StatusMessageView_normal : NSView
{
  IBOutlet NSTextField* message_;
}

- (void) setMessage:(NSString*)message;

@end
