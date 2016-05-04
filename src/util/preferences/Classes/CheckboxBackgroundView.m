#import "CheckboxBackgroundView.h"

@implementation CheckboxBackgroundView

- (void)drawRect:(NSRect)dirtyRect {
  [NSGraphicsContext saveGraphicsState];
  {
    NSRect bounds = [self bounds];
    [self.color set];
    [[NSBezierPath bezierPathWithRoundedRect:bounds xRadius:5 yRadius:5] fill];
  }
  [NSGraphicsContext restoreGraphicsState];
}

@end
