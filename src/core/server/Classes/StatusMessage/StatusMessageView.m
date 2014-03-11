// -*- Mode: objc -*-

#import "PreferencesKeys.h"
#import "StatusMessageView.h"

@implementation StatusMessageView

- (void) setMessage:(NSString*)message
{
  [message_ setStringValue:message];
  [self setNeedsDisplay:YES];
}

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
        // Black
        [message_ setTextColor:[NSColor whiteColor]];
        [[NSColor blackColor] set];
        break;

      case 3:
        // Blue
        [message_ setTextColor:[NSColor whiteColor]];
        [[NSColor blueColor] set];
        break;

      case 4:
        // Brown
        [message_ setTextColor:[NSColor whiteColor]];
        [[NSColor brownColor] set];
        break;

      case 5:
        // Cyan
        [message_ setTextColor:[NSColor blackColor]];
        [[NSColor cyanColor] set];
        break;

      case 6:
        // Green
        [message_ setTextColor:[NSColor blackColor]];
        [[NSColor greenColor] set];
        break;

      case 7:
        // Magenta
        [message_ setTextColor:[NSColor whiteColor]];
        [[NSColor magentaColor] set];
        break;

      case 8:
        // Orange
        [message_ setTextColor:[NSColor blackColor]];
        [[NSColor orangeColor] set];
        break;

      case 9:
        // Purple
        [message_ setTextColor:[NSColor whiteColor]];
        [[NSColor purpleColor] set];
        break;

      case 10:
        // Red
        [message_ setTextColor:[NSColor whiteColor]];
        [[NSColor redColor] set];
        break;

      case 11:
        // Yellow
        [message_ setTextColor:[NSColor blackColor]];
        [[NSColor yellowColor] set];
        break;

      default:
        // White
        [message_ setTextColor:[NSColor blackColor]];
        [[NSColor whiteColor] set];
        break;
    }
    [[NSBezierPath bezierPathWithRoundedRect:bounds xRadius:10 yRadius:10] fill];
  }
  [NSGraphicsContext restoreGraphicsState];
}

- (void) updateWindowFrame
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSInteger position = [defaults integerForKey:kStatusWindowPosition];

  NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
  NSRect windowFrame = [[self window] frame];
  int margin = 10;
  NSPoint point;

  switch (position) {
    case 0:
      // Top left
      point.x = screenFrame.origin.x + margin;
      point.y = screenFrame.origin.y + screenFrame.size.height - windowFrame.size.height - margin;
      break;
    case 1:
      // Top right
      point.x = screenFrame.origin.x + screenFrame.size.width  - windowFrame.size.width - margin;
      point.y = screenFrame.origin.y + screenFrame.size.height - windowFrame.size.height - margin;
      break;
    case 2:
      // Bottom left
      point.x = screenFrame.origin.x + margin;
      point.y = screenFrame.origin.y + margin;
      break;
    case 3:
    default:
      // Bottom right
      point.x = screenFrame.origin.x + screenFrame.size.width  - windowFrame.size.width - margin;
      point.y = screenFrame.origin.y + margin;
      break;
  }

  [[self window] setFrameOrigin:point];
}

@end
