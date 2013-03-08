// -*- Mode: objc -*-

#import "PreferencesKeys.h"
#import "StatusMessageView_edge.h"

@implementation StatusMessageView_edge

- (void) updateWindowFrame
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSInteger position = [defaults integerForKey:kStatusWindowPosition];

  CGFloat width = [[NSApp mainMenu] menuBarHeight] / 4;

  NSRect rect = [[NSScreen mainScreen] frame];
  switch (position) {
    case 0:
      // Top left -> Top
      rect.origin.y += rect.size.height - width;
      rect.size.height = width;
      break;
    case 1:
      // Top right -> Right
      rect.origin.x += rect.size.width - width;
      rect.size.width = width;
      break;
    case 2:
      // Bottom left -> Left
      rect.size.width = width;
      break;
    case 3:
    default:
      // Bottom right -> Bottom
      rect.size.height = width;
      break;
  }

  [[self window] setFrame:rect display:NO];
}

@end
