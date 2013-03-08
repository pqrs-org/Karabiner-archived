// -*- Mode: objc -*-

#import "PreferencesKeys.h"
#import "StatusMessageView_normal.h"

@implementation StatusMessageView_normal

- (void) drawRect:(NSRect)dirtyRect
{
  [super drawRect:dirtyRect];

  // Font Size
  NSInteger fontsize = [[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowFontSize];
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

@end
