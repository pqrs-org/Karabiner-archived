#import "AXApplicationObserver.h"
#import "AXApplicationObserverManager.h"

@interface AXApplicationObserverManager ()
{
  NSMutableDictionary* observers_;

  NSTimer* timer_;
  NSRunningApplication* runningApplicationForAXApplicationObserver_;
  int retryCounter_;
}
@end

@implementation AXApplicationObserverManager

- (void) timerFireMethod:(NSTimer*)timer
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      if (! runningApplicationForAXApplicationObserver_) {
        retryCounter_ = 0;

      } else {
        if (retryCounter_ > 20) {
          runningApplicationForAXApplicationObserver_ = nil;
          return;
        }
        if (retryCounter_ > 0) {
          NSLog(@"AXApplicationObserverManager creates AXApplicationObserver for %@ (retryCounter_:%d)",
                runningApplicationForAXApplicationObserver_,
                retryCounter_);
        }

        @try {
          AXApplicationObserver* o = [[AXApplicationObserver alloc] initWithRunningApplication:runningApplicationForAXApplicationObserver_];
          pid_t pid = [runningApplicationForAXApplicationObserver_ processIdentifier];
          observers_[@(pid)] = o;

          [o observeTitleChangedNotification];
          [o postNotification];

          runningApplicationForAXApplicationObserver_ = nil;
          retryCounter_ = 0;

        } @catch (NSException* e) {
          NSLog(@"%@", e);
          ++retryCounter_;
        }
      }
    }
  });
}

- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      for (NSNumber* pid in observers_) {
        [observers_[pid] unobserveTitleChangedNotification];
      }

      runningApplicationForAXApplicationObserver_ = [notification userInfo][NSWorkspaceApplicationKey];
      [timer_ fire];
    }
  });
}

- (instancetype) init
{
  self = [super init];

  if (self) {
    observers_ = [NSMutableDictionary new];

    [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                           selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                               name:NSWorkspaceDidActivateApplicationNotification
                                                             object:nil];

    for (NSRunningApplication* runningApplication in [[NSWorkspace sharedWorkspace] runningApplications]) {
      @try {
        pid_t pid = [runningApplication processIdentifier];
        AXApplicationObserver* app = [[AXApplicationObserver alloc] initWithRunningApplication:runningApplication];
        observers_[@(pid)] = app;
      } @catch (NSException* e) {
        NSLog(@"%@", e);
      }
    }

    // ----------------------------------------
    timer_ = [NSTimer scheduledTimerWithTimeInterval:0.5
                                              target:self
                                            selector:@selector(timerFireMethod:)
                                            userInfo:nil
                                             repeats:YES];
    runningApplicationForAXApplicationObserver_ = [[NSWorkspace sharedWorkspace] frontmostApplication];
    [timer_ fire];
  }

  return self;
}

@end
