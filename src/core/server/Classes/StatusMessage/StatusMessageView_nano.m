// -*- Mode: objc -*-

#import "PreferencesKeys.h"
#import "StatusMessageView_nano.h"

@implementation StatusMessageView_nano

- (void) drawRect:(NSRect)dirtyRect
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSInteger theme = [defaults integerForKey:kStatusWindowTheme];
  double opacity = [defaults doubleForKey:kStatusWindowOpacity];

  // Opacity
  [self setAlphaValue:(opacity / 100)];

  // Background
  [NSGraphicsContext saveGraphicsState];
  {
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
