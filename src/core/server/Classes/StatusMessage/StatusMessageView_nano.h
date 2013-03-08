// -*- Mode: objc -*-

#import "StatusMessageView.h"

@interface StatusMessageView_nano : StatusMessageView
{
  IBOutlet NSTextField* message_;
}

- (void) setMessage:(NSString*)message;

@end
