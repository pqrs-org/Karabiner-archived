#import "AppDelegate.h"
#import "KeyRemap4MacBookKeys.h"

// ==================================================
@interface AppDelegate ()
{
  BOOL initialized_;
  AXObserverRef observer_;
}
@end

@implementation AppDelegate

static void observerCallback(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void* refcon)
{
  if (CFStringCompare(notification, kAXTitleChangedNotification, 0) == kCFCompareEqualTo) {
    [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKeyRemap4MacBookAXTitleChangedNotification
                                                                   object:nil];
  }
}

- (void) registerApplication:(NSRunningApplication*)runningApplication
{
  // ----------------------------------------
  // unregister
  if (observer_) {
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(),
                          AXObserverGetRunLoopSource(observer_),
                          kCFRunLoopDefaultMode);

    CFRelease(observer_);
    observer_ = NULL;
  }

  // ----------------------------------------
  if (! AXIsProcessTrusted()) return;

  // ----------------------------------------
  AXError error = kAXErrorSuccess;
  AXUIElementRef application = NULL;

  pid_t pid = [runningApplication processIdentifier];

  error = AXObserverCreate(pid, observerCallback, &observer_);
  if (error != kAXErrorSuccess) {
    NSLog(@"AXObserverCreate is failed. pid:%d error:%d", pid, error);
    goto finish;
  }

  application = AXUIElementCreateApplication(pid);
  if (! application) {
    NSLog(@"AXUIElementCreateApplication is failed. pid:%d", pid);
    goto finish;
  }

  error = AXObserverAddNotification(observer_, application, kAXTitleChangedNotification, (__bridge void*)self);
  if (error != kAXErrorSuccess) {
    NSLog(@"AXObserverAddNotification is failed: pid:%d error:%d", pid, error);
    goto finish;
  }

  CFRunLoopAddSource(CFRunLoopGetCurrent(),
                     AXObserverGetRunLoopSource(observer_),
                     kCFRunLoopDefaultMode);

finish:
  if (application) {
    CFRelease(application);
    application = NULL;
  }
}

- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  NSRunningApplication* runningApplication = [notification userInfo][NSWorkspaceApplicationKey];
  [self registerApplication:runningApplication];
}

- (void) timerFireMethod:(NSTimer*)timer
{
  dispatch_async(dispatch_get_main_queue(), ^{
    if (AXIsProcessTrusted()) {
      [_text setStringValue:@"AXNotifier is working."];
      [[NSApplication sharedApplication] hide:self];

      if (! initialized_) {
        initialized_ = YES;
        [self registerApplication:[[NSWorkspace sharedWorkspace] frontmostApplication]];
      }

    } else {
      [_text setStringValue:@"AXNotifier requires using the accessibility features in order to detect window's title changes.\nPlease permit the accessibility features."];

      initialized_ = NO;
    }
  });
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSDictionary* options = @{ (__bridge NSString*)(kAXTrustedCheckOptionPrompt): @YES };
  AXIsProcessTrustedWithOptions((__bridge CFDictionaryRef)options);

  if (! AXIsProcessTrusted()) {
    [_window orderFront:self];
  }

  [NSTimer scheduledTimerWithTimeInterval:1.0
                                   target:self
                                 selector:@selector(timerFireMethod:)
                                 userInfo:nil
                                  repeats:YES];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];
}

- (BOOL) applicationShouldHandleReopen:(NSApplication*)theApplication hasVisibleWindows:(BOOL)flag
{
  [[NSApplication sharedApplication] unhide:self];
  return YES;
}

@end
