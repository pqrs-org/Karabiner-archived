// -*- Mode: objc -*-

#import "StatusMessageView.h"

@implementation StatusMessageView

- (void) drawRect:(NSRect)dirtyRect
{
  [NSGraphicsContext saveGraphicsState];
  {
    NSRect bounds = [self bounds];

    // Draw bounds
    [[NSColor whiteColor] set];
    [[NSBezierPath bezierPathWithRoundedRect:bounds xRadius:10 yRadius:10] fill];
  }
  [NSGraphicsContext restoreGraphicsState];
}

- (void) setMessage:(NSString*)message
{
  [message_ setStringValue:message];
  [self setNeedsDisplay:YES];
}

@end
