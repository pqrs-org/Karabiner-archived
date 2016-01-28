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

- (void)setLayoutConstraint {
  NSLayoutAttribute attributes[] = {
      NSLayoutAttributeTop,
      NSLayoutAttributeBottom,
      NSLayoutAttributeLeading,
      NSLayoutAttributeTrailing,
  };

  for (size_t i = 0; i < sizeof(attributes) / sizeof(attributes[0]); ++i) {
    [self.superview addConstraint:[NSLayoutConstraint constraintWithItem:self
                                                               attribute:attributes[i]
                                                               relatedBy:NSLayoutRelationEqual
                                                                  toItem:self.superview
                                                               attribute:attributes[i]
                                                              multiplier:1.0
                                                                constant:0]];
  }
}

@end
