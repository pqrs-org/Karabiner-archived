// -*- Mode: objc -*-

#import "PreferencesKeys.h"
#import "StatusMessageView.h"

@implementation StatusMessageView

- (void) drawRect:(NSRect)dirtyRect
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSInteger theme = [defaults integerForKey:kStatusWindowTheme];
  double opacity = [defaults doubleForKey:kStatusWindowOpacity];
  NSInteger fontsize = [defaults integerForKey:kStatusWindowFontSize];

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

  // Font Size
  switch (fontsize) {
    case 1: // Regular
      [message_ setFont:[NSFont systemFontOfSize:[NSFont systemFontSize]]];
      break;

    case 2: // Large
      [message_ setFont:[NSFont systemFontOfSize:([NSFont smallSystemFontSize] * 3)]];
      break;

    case 3: // Huge
      [message_ setFont:[NSFont systemFontOfSize:([NSFont smallSystemFontSize] * 4)]];
      break;

    default: // Small
      [message_ setFont:[NSFont systemFontOfSize:[NSFont smallSystemFontSize]]];
      break;
  }
}

- (void) setMessage:(NSString*)message
{
  [message_ setStringValue:message];
  [self setNeedsDisplay:YES];
}

@end
