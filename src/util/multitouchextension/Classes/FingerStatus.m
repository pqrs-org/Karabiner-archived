#import "FingerStatus.h"

@implementation FingerStatus

- (id) init
{
  self = [super init];

  if (self) {
    [self clear];
  }

  return self;
}

- (void) clear
{
  size_ = 0;
}

- (void) add:(int)identifier active:(BOOL)active
{
  if (size_ < FINGER_STATUS_MAX_FINGERS) {
    identifier_[size_] = identifier;
    active_[size_] = active;
    ++size_;
  }
}

- (BOOL) isActive:(int)identifier
{
  for (size_t i = 0; i < size_; ++i) {
    if (identifier_[i] == identifier) {
      return active_[i];
    }
  }
  return NO;
}

@end
