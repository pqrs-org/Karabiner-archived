#import "SessionObserver.h"
#import "weakify.h"
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
    _lastState = NO;
    _active = active;
    _inactive = inactive;

    _timer = [NSTimer scheduledTimerWithTimeInterval:1
                                              target:self
                                            selector:@selector(timerFireMethod:)
                                            userInfo:nil
                                             repeats:YES];
    [_timer fire];
  }

  return self;
}

- (void)dealloc {
  [_timer invalidate];
}

- (void)timerFireMethod:(NSTimer*)timer {
  // NSWorkspaceSessionDidResignActiveNotification is sometimes not called. (OS X bug?)
  // Therefore, we have to check session state in timer.

  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

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
