// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>

@interface FingerStatus : NSObject
{
  enum {
    FINGER_STATUS_MAX_FINGERS = 20,
  };

  int identifier_[FINGER_STATUS_MAX_FINGERS];
  BOOL active_[FINGER_STATUS_MAX_FINGERS];
  size_t size_;
}

- (id) init;
- (void) clear;
- (void) add:(int)identifier active:(BOOL)active;
- (BOOL) isActive:(int)identifier;

@end
