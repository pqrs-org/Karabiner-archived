// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "PreferencesController.h"

@interface IgnoredAreaView : NSView
{
  enum {
    IGNORE_AREA_VIEW_MAX_FINGERS = 20,
  };
  NSPoint fingers_[IGNORE_AREA_VIEW_MAX_FINGERS];
  size_t nfingers_;
}

- (void) clearFingers;
- (void) addFinger:(NSPoint)point;
+ (BOOL) isIgnoredArea:(NSPoint)point;

- (IBAction) draw:(id)sender;

@end
