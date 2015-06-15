#import "SessionObserver.h"
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

@interface SessionObserver () {
  NSTimer* timer_;
  BOOL lastState_;

  void (^active_)(void);
  void (^inactive_)(void);
}
@end

@implementation SessionObserver

- (instancetype)init:(NSTimeInterval)seconds
              active:(void (^)(void))active
            inactive:(void (^)(void))inactive {
  self = [super init];

  if (self) {
    lastState_ = NO;
    active_ = active;
    inactive_ = inactive;

    timer_ = [NSTimer scheduledTimerWithTimeInterval:1
                                              target:self
                                            selector:@selector(timerFireMethod:)
                                            userInfo:nil
                                             repeats:YES];
    [timer_ fire];
  }

  return self;
}

- (void)dealloc {
  [timer_ invalidate];
}

- (void)timerFireMethod:(NSTimer*)timer {
  // NSWorkspaceSessionDidResignActiveNotification is sometimes not called. (OS X bug?)
  // Therefore, we have to check session state in timer.

  dispatch_async(dispatch_get_main_queue(), ^{
    BOOL currentState = [self isUserActive];
    if (lastState_ != currentState) {
      NSLog(@"Session state has been changed. (%s)", currentState ? "active" : "inactive");
      lastState_ = currentState;

      if (currentState) {
        if (active_) {
          active_();
        }

      } else {
        if (inactive_) {
          inactive_();
        }
      }
    }
  });
}

- (BOOL)isUserActive {
  BOOL result = NO;
  CFDictionaryRef sessionInfoDict = CGSessionCopyCurrentDictionary();

  if (sessionInfoDict) {
    CFBooleanRef userIsActive = CFDictionaryGetValue(sessionInfoDict, kCGSessionOnConsoleKey);
    if (userIsActive) {
      result = CFBooleanGetValue(userIsActive);
    }
    CFRelease(sessionInfoDict);
  }

  return result;
}

@end
