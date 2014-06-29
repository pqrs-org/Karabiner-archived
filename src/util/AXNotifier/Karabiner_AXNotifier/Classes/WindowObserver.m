#import "NotificationKeys.h"
#import "WindowObserver.h"

@interface WindowObserver ()
{
  NSTimer* timer_;
  NSMutableDictionary* shown_;
}
@end

@implementation WindowObserver : NSObject

- (instancetype) init
{
  self = [super init];

  if (self) {
    shown_ = [NSMutableDictionary new];

    timer_ = [NSTimer scheduledTimerWithTimeInterval:0.5
                                              target:self
                                            selector:@selector(timerFireMethod:)
                                            userInfo:nil
                                             repeats:YES];

    [timer_ fire];
  }

  return self;
}

- (void) dealloc
{
  [timer_ invalidate];
}

- (void) timerFireMethod:(NSTimer*)timer
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      [self checkWindowsShown];
    }
  });
}

- (void) checkWindowsShown
{
  NSArray* windows = (__bridge_transfer NSArray*)(CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly,
                                                                             kCGNullWindowID));
  // Check windows state in priority order.
  for (NSDictionary* window in windows) {
    if ([window[(__bridge NSString*)(kCGWindowOwnerName)] isEqualToString:@"Dock"] &&
        [window[(__bridge NSString*)(kCGWindowName)] isEqualToString:@"Launchpad"]) {
      if (! shown_[@"Launchpad"]) {
        shown_[@"Launchpad"] = @YES;
        [self postNotification:@"Launchpad" visibility:YES];
        return;
      }
    }
  }

  for (NSString* key in shown_) {
    if (shown_[key]) {
      [self postNotification:key visibility:NO];
      [shown_ removeObjectForKey:key];
    }
  }
}

- (void) postNotification:(NSString*)windowName visibility:(BOOL)visibility
{
  NSDictionary* userInfo = @{
    @"windowName": windowName,
    @"visibility": @(visibility),
  };
  [[NSNotificationCenter defaultCenter] postNotificationName:kWindowVisibilityChanged object:self userInfo:userInfo];
}

@end
