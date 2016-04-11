#import "SessionObserver.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>

@interface SessionObserver ()

@property NSTimer* timer;
@property BOOL lastState;
@property(copy) void (^active)(void);
@property(copy) void (^inactive)(void);

@end

@implementation SessionObserver

- (instancetype)init:(NSTimeInterval)seconds
              active:(void (^)(void))active
            inactive:(void (^)(void))inactive {
  self = [super init];

  if (self) {
    self.lastState = NO;
    self.active = active;
    self.inactive = inactive;

    self.timer = [NSTimer scheduledTimerWithTimeInterval:1
                                                  target:self
                                                selector:@selector(timerFireMethod:)
                                                userInfo:nil
                                                 repeats:YES];
    [self.timer fire];
  }

  return self;
}

- (void)dealloc {
  [self.timer invalidate];
}

- (void)timerFireMethod:(NSTimer*)timer {
  // NSWorkspaceSessionDidResignActiveNotification is sometimes not called. (OS X bug?)
  // Therefore, we have to check session state in timer.

  dispatch_async(dispatch_get_main_queue(), ^{
    BOOL currentState = [self isUserActive];
    if (self.lastState != currentState) {
      NSLog(@"Session state has been changed. (%s)", currentState ? "active" : "inactive");
      self.lastState = currentState;

      if (currentState) {
        if (self.active) {
          self.active();
        }

      } else {
        if (self.inactive) {
          self.inactive();
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
