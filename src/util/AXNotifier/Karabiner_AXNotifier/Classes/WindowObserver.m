#import "NotificationKeys.h"
#import "WindowObserver.h"

enum {
  WINDOWID_LAUNCHPAD,
  WINDOWID_SPOTLIGHT,
  WINDOWID__END__,
};

@interface WindowObserver () {
  NSTimer* timer_;
  NSTimer* refreshWindowIDsTimer_;
  NSMutableDictionary* shown_;

  // rawWindowIDs_ should be pointer sized type in order to pass CFArrayCreate.
  long rawWindowIDs_[WINDOWID__END__];
  CFArrayRef windowIDs_;
}
@end

@implementation WindowObserver : NSObject

- (instancetype)init {
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

- (void)dealloc {
  [timer_ invalidate];

  if (windowIDs_) {
    CFRelease(windowIDs_);
  }
}

- (BOOL)isLaunchpad:(NSString*)windowOwnerName
         windowName:(NSString*)windowName
        windowLayer:(NSInteger)windowLayer {
  // For OS X 10.9.
  if ([windowOwnerName isEqualToString:@"Dock"] &&
      [windowName isEqualToString:@"Launchpad"]) {
    return YES;
  }

  // For OS X 10.10.
  //
  // Dock has two windows (Dock and Launchpad).
  // If kCGWindowName is "Dock", it is the Dock window.
  // We need to ignore the Dock window.
  //
  // --------------------------------------------------
  //
  // When Launchpad is shown:
  //
  // com.apple.dock
  // {
  //     kCGWindowAlpha = 1;
  //     kCGWindowBounds =     {
  //         Height = 1200;
  //         Width = 1920;
  //         X = 0;
  //         Y = 0;
  //     };
  //     kCGWindowIsOnscreen = 1;
  //     kCGWindowLayer = 27;
  //     kCGWindowMemoryUsage = 1008;
  //     kCGWindowNumber = 54;
  //     kCGWindowOwnerName = Dock;
  //     kCGWindowOwnerPID = 300;
  //     kCGWindowSharingState = 1;
  //     kCGWindowStoreType = 1;
  // }
  //
  // --------------------------------------------------
  //
  // When Launchpad is hidden:
  //
  // com.apple.dock
  // {
  //     kCGWindowAlpha = 1;
  //     kCGWindowBounds =     {
  //         Height = 1200;
  //         Width = 1920;
  //         X = 0;
  //         Y = 0;
  //     };
  //     kCGWindowLayer = 20;
  //     kCGWindowMemoryUsage = 1008;
  //     kCGWindowNumber = 54;
  //     kCGWindowOwnerName = Dock;
  //     kCGWindowOwnerPID = 300;
  //     kCGWindowSharingState = 1;
  //     kCGWindowStoreType = 1;
  // }
  //
  // --------------------------------------------------
  //
  // The Dock window
  //
  // com.apple.dock
  // {
  //     kCGWindowAlpha = 1;
  //     kCGWindowBounds =     {
  //         Height = 1200;
  //         Width = 1920;
  //         X = 0;
  //         Y = 0;
  //     };
  //     kCGWindowIsOnscreen = 1;
  //     kCGWindowLayer = 20;
  //     kCGWindowMemoryUsage = 1008;
  //     kCGWindowName = Dock;
  //     kCGWindowNumber = 55;
  //     kCGWindowOwnerName = Dock;
  //     kCGWindowOwnerPID = 300;
  //     kCGWindowSharingState = 1;
  //     kCGWindowStoreType = 1;
  // }

  if ([windowOwnerName isEqualToString:@"Dock"] &&
      windowName == nil) {
    return YES;
  }

  return NO;
}

- (BOOL)isSpotlight:(NSString*)windowOwnerName
         windowName:(NSString*)windowName
        windowLayer:(NSInteger)windowLayer {
  if ([windowOwnerName isEqualToString:@"Spotlight"] &&
      [windowName isEqualToString:@"Spotlight"]) {

    // There is no reliable public specifications for kCGWindowLayer.
    // So, we use magic numbers that are confirmed by "warp-mouse-cursor-position".

    // Ignore Spotlight in statusbar.
    if (0 < windowLayer && windowLayer < 25) {
      return YES;
    }
  }

  return NO;
}

- (void)refreshWindowIDsTimerFireMethod:(NSTimer*)timer {
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
        NSInteger windowLayer = [window[(__bridge NSString*)(kCGWindowLayer)] integerValue];

        if ([self isLaunchpad:windowOwnerName
                   windowName:windowName
                  windowLayer:windowLayer]) {
          rawWindowIDs_[WINDOWID_LAUNCHPAD] = [window[(__bridge NSString*)(kCGWindowNumber)] unsignedIntValue];
        }

        if ([self isSpotlight:windowOwnerName
                   windowName:windowName
                  windowLayer:windowLayer]) {
          rawWindowIDs_[WINDOWID_SPOTLIGHT] = [window[(__bridge NSString*)(kCGWindowNumber)] unsignedIntValue];
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

- (void)timerFireMethod:(NSTimer*)timer {
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      if (windowIDs_) {
        NSArray* windows = (__bridge_transfer NSArray*)(CGWindowListCreateDescriptionFromArray(windowIDs_));
        for (NSDictionary* window in windows) {
          pid_t windowOwnerPID = [window[(__bridge NSString*)(kCGWindowOwnerPID)] intValue];
          long windowNumber = [window[(__bridge NSString*)(kCGWindowNumber)] unsignedIntValue];
          BOOL isOnScreen = [window[(__bridge NSString*)(kCGWindowIsOnscreen)] boolValue];

          NSString* key = NULL;

          if (rawWindowIDs_[WINDOWID_LAUNCHPAD] == windowNumber) {
            key = @"Launchpad";
          }
          if (rawWindowIDs_[WINDOWID_SPOTLIGHT] == windowNumber) {
            key = @"Spotlight";
          }

          if (key) {
            if (isOnScreen) {
              if (! shown_[key]) {
                NSString* bundleIdentifier = [[NSRunningApplication runningApplicationWithProcessIdentifier:windowOwnerPID] bundleIdentifier];
                if (bundleIdentifier) {
                  shown_[key] = bundleIdentifier;
                  [self postNotification:key bundleIdentifier:shown_[key] visibility:YES];
                }
              }
              return;
            }
          }
        }
      }

      // ----------------------------------------
      // There is no target window in screen.

      for (NSString* key in shown_) {
        if (shown_[key]) {
          [self postNotification:key bundleIdentifier:shown_[key] visibility:NO];
        }
      }
      [shown_ removeAllObjects];
    }
  });
}

- (void)postNotification:(NSString*)windowName bundleIdentifier:(NSString*)bundleIdentifier visibility:(BOOL)visibility {
  NSDictionary* userInfo = @{
    @"bundleIdentifier" : bundleIdentifier,
    @"windowName" : windowName,
    @"role" : @"",
    @"visibility" : @(visibility),
  };
  [[NSNotificationCenter defaultCenter] postNotificationName:kWindowVisibilityChanged object:self userInfo:userInfo];
}

@end
