// -*- Mode: objc -*-

#import "PreferencesKeys.h"
#import "StatusMessageView.h"

@implementation StatusMessageView

- (void) drawRect:(NSRect)dirtyRect
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSInteger theme = [defaults integerForKey:kStatusWindowTheme];
  double opacity = [defaults doubleForKey:kStatusWindowOpacity];

  [self setAlphaValue:(opacity / 100)];

  [NSGraphicsContext saveGraphicsState];
  {
    // Draw bounds
    NSRect bounds = [self bounds];
    switch (theme) {
      case 1:
        [message_ setTextColor:[NSColor whiteColor]];
        [[NSColor blackColor] set];
        break;

      default:
        [message_ setTextColor:[NSColor blackColor]];
        [[NSColor whiteColor] set];
        break;
    }
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
