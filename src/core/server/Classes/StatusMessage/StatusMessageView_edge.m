// -*- Mode: objc -*-

#import "StatusMessageView_edge.h"
#import "PreferencesKeys.h"
#import "PreferencesModel.h"

@implementation StatusMessageView_edge

- (void)updateWindowFrame:(NSScreen*)screen {
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSInteger position = [defaults integerForKey:kStatusWindowPosition];

  CGFloat width = [[NSApp mainMenu] menuBarHeight] / 8;
  switch (self.preferencesModel.statusWindowFontSize) {
  case 1: // Regular
    width *= 2;
    break;
  case 2: // Large
    width *= 4;
    break;
  case 3: // Huge
    width *= 8;
    break;
  default: // Small
    break;
  }

  NSRect rect = [screen frame];
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
