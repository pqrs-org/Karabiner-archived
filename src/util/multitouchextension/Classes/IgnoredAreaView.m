// -*- Mode: objc -*-

#import "IgnoredAreaView.h"

@implementation IgnoredAreaView

+ (NSRect) getTargetArea
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  double top    = [[defaults stringForKey:@"ignoredAreaTop"]    doubleValue] / 100;
  double bottom = [[defaults stringForKey:@"ignoredAreaBottom"] doubleValue] / 100;
  double left   = [[defaults stringForKey:@"ignoredAreaLeft"]   doubleValue] / 100;
  double right  = [[defaults stringForKey:@"ignoredAreaRight"]  doubleValue] / 100;

  return NSMakeRect(left,
                    bottom,
                    (1.0 - left - right),
                    (1.0 - top - bottom));
}

- (void) drawRect:(NSRect)dirtyRect
{
  [NSGraphicsContext saveGraphicsState];
  {
    NSRect bounds = [self bounds];

    // Draw bounds
    [[[NSColor grayColor] colorWithAlphaComponent:0.5] set];
    [[NSBezierPath bezierPathWithRoundedRect:bounds xRadius:10 yRadius:10] fill];

    // Draw target area
    NSRect targetArea = [IgnoredAreaView getTargetArea];

    [[NSColor grayColor] set];
    [[NSBezierPath bezierPathWithRoundedRect:NSMakeRect(bounds.size.width  * targetArea.origin.x,
                                                        bounds.size.height * targetArea.origin.y,
                                                        bounds.size.width  * targetArea.size.width,
                                                        bounds.size.height * targetArea.size.height)
                                     xRadius:10 yRadius:10] fill];

    // Draw fingers
    for (size_t i = 0; i < nfingers_; ++i) {
      enum {
        DIAMETER = 10,
      };

      if ([IgnoredAreaView isIgnoredArea:fingers_[i]]) {
        [[NSColor blueColor] set];
      } else {
        [[NSColor redColor] set];
      }
      NSRect rect = NSMakeRect(bounds.size.width  * fingers_[i].x - DIAMETER / 2,
                               bounds.size.height * fingers_[i].y - DIAMETER / 2,
                               DIAMETER,
                               DIAMETER);
      NSBezierPath* path = [NSBezierPath bezierPathWithOvalInRect:rect];
      [path setLineWidth:2];
      [path stroke];
    }
  }
  [NSGraphicsContext restoreGraphicsState];
}

- (void) clearFingers
{
  nfingers_ = 0;
  [self setNeedsDisplay:YES];
}

- (void) addFinger:(NSPoint)point
{
  fingers_[nfingers_] = point;

  if (nfingers_ < IGNORE_AREA_VIEW_MAX_FINGERS - 1) {
    ++nfingers_;
  }
  [self setNeedsDisplay:YES];
}

+ (BOOL) isIgnoredArea:(NSPoint)point
{
  NSRect targetArea = [IgnoredAreaView getTargetArea];
  return ! NSPointInRect(point, targetArea);
}

- (IBAction) draw:(id)sender
{
  [self setNeedsDisplay:YES];
}

@end
