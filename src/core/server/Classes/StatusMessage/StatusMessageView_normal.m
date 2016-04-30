// -*- Mode: objc -*-

#import "StatusMessageView_normal.h"
#import "PreferencesModel.h"

@implementation StatusMessageView_normal

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];

  // Font Size
  switch (self.preferencesModel.statusWindowFontSize) {
  case 1: // Regular
    [self.message setFont:[NSFont systemFontOfSize:[NSFont systemFontSize]]];
    break;

  case 2: // Large
    [self.message setFont:[NSFont systemFontOfSize:([NSFont smallSystemFontSize] * 3)]];
    break;

  case 3: // Huge
    [self.message setFont:[NSFont systemFontOfSize:([NSFont smallSystemFontSize] * 4)]];
    break;

  default: // Small
    [self.message setFont:[NSFont systemFontOfSize:[NSFont smallSystemFontSize]]];
    break;
  }
}

@end
