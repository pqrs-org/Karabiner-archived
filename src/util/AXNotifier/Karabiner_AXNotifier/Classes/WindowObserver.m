#import "NotificationKeys.h"
#import "WindowObserver.h"

enum {
  WINDOWID_LAUNCHPAD,
  WINDOWID__END__,
};

@interface WindowObserver ()
{
  NSTimer* timer_;
  NSTimer* refreshWindowIDsTimer_;
  NSMutableDictionary* shown_;

  // rawWindowIDs_ should be pointer sized type in order to pass CFArrayCreate.
  long rawWindowIDs_[WINDOWID__END__];
  CFArrayRef windowIDs_;
}
@end

@implementation WindowObserver : NSObject

- (instancetype) init
{
  self = [super init];

  if (self) {
    shown_ = [NSMutableDictionary new];

    refreshWindowIDsTimer_ = [NSTimer scheduledTimerWithTimeInterval:10
                                                              target:self
                                                            selector:@selector(refreshWindowIDsTimerFireMethod:)
                                                            userInfo:nil
                                                             repeats:YES];

    timer_ = [NSTimer scheduledTimerWithTimeInterval:0.5
                                              target:self
                                            selector:@selector(timerFireMethod:)
                                            userInfo:nil
                                             repeats:YES];

    [refreshWindowIDsTimer_ fire];
    [timer_ fire];
  }

  return self;
}

- (void) dealloc
{
  [timer_ invalidate];

  if (windowIDs_) {
    CFRelease(windowIDs_);
  }
}

- (void) refreshWindowIDsTimerFireMethod:(NSTimer*)timer
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      // ----------------------------------------
      // update rawWindowIDs_

      for (size_t i = 0; i < WINDOWID__END__; ++i) {
        rawWindowIDs_[i] = 0;
      }

      NSArray* windows = (__bridge_transfer NSArray*)(CGWindowListCopyWindowInfo(kCGWindowListOptionAll,
                                                                                 kCGNullWindowID));
      for (NSDictionary* window in windows) {
        NSString* windowOwnerName = window[(__bridge NSString*)(kCGWindowOwnerName)];
        NSString* windowName = window[(__bridge NSString*)(kCGWindowName)];

        if ([windowOwnerName isEqualToString:@"Dock"] &&
            [windowName isEqualToString:@"Launchpad"]) {
          rawWindowIDs_[WINDOWID_LAUNCHPAD] = [window[(__bridge NSString*)(kCGWindowNumber)] unsignedIntValue];
        }
      }

      // ----------------------------------------
      // update windowIDs_

      if (windowIDs_) {
        CFRelease(windowIDs_);
        windowIDs_ = NULL;
      }
      windowIDs_ = CFArrayCreate(NULL, (const void**)(rawWindowIDs_), WINDOWID__END__, NULL);
    }
  });
}

- (void) timerFireMethod:(NSTimer*)timer
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      if (windowIDs_) {
        NSArray* windows = (__bridge_transfer NSArray*)(CGWindowListCreateDescriptionFromArray(windowIDs_));
        for (NSDictionary* window in windows) {
          pid_t windowOwnerPID = [window[(__bridge NSString*)(kCGWindowOwnerPID)] intValue];
          NSString* windowOwnerName = window[(__bridge NSString*)(kCGWindowOwnerName)];
          NSString* windowName = window[(__bridge NSString*)(kCGWindowName)];
          BOOL isOnScreen = [window[(__bridge NSString*)(kCGWindowIsOnscreen)] boolValue];

          if ([windowOwnerName isEqualToString:@"Dock"] &&
              [windowName isEqualToString:@"Launchpad"]) {
            NSString* key = @"Launchpad";
            if (isOnScreen) {
              if (! shown_[key]) {
                shown_[key] = [[NSRunningApplication runningApplicationWithProcessIdentifier:windowOwnerPID] bundleIdentifier];
                [self postNotification:key bundleIdentifier:shown_[key] visibility:YES];
              }
              return;
            }
          }
        }
      }

      for (NSString* key in shown_) {
        if (shown_[key]) {
          [self postNotification:key bundleIdentifier:shown_[key] visibility:NO];
          [shown_ removeObjectForKey:key];
        }
      }
    }
  });
}

- (void) postNotification:(NSString*)windowName bundleIdentifier:(NSString*)bundleIdentifier visibility:(BOOL)visibility
{
  NSDictionary* userInfo = @{
    @"bundleIdentifier": bundleIdentifier,
    @"windowName": windowName,
    @"role": @"",
    @"visibility": @(visibility),
  };
  [[NSNotificationCenter defaultCenter] postNotificationName:kWindowVisibilityChanged object:self userInfo:userInfo];
}

@end
