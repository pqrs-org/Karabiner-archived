#import "AXUtilities.h"
#import "AppDelegate.h"
#import "KeyRemap4MacBookKeys.h"
#import "PreferencesKeys.h"

// ==================================================
@interface AppDelegate ()
{
  BOOL initialized_;
  AXObserverRef observer_;
  NSMutableDictionary* focusedUIElementInformation_;
}
@end

@implementation AppDelegate

/*
 * Notification and Information
 * ============================
 *
 * We need to observe these notifications on a web browser which has tab and multi window.
 *
 * - NSWorkspaceDidActivateApplicationNotification
 * - kAXTitleChangedNotification
 * - kAXFocusedWindowChangedNotification
 * - kAXFocusedUIElementChangedNotification
 *
 *
 * NSWorkspaceDidActivateApplicationNotification
 * ---------------------------------------------
 *
 *   A essential notification for bundle identifier.
 *   This notification is also needed for title and role when user changed focused application.
 *
 *   (kAXTitleChangedNotification and other AX notifications will not be sent when user changed focused application
 *   because we create an observer for focused application when application is focused.)
 *
 *   Items that need to be updated:
 *     - bundleIdentifier
 *     - title
 *     - role
 *
 *
 * kAXTitleChangedNotification
 * ---------------------------
 *
 *   A essential notification for window name.
 *
 *   Items that need to be updated:
 *     - title
 *
 *
 * kAXFocusedWindowChangedNotification
 * -----------------------------------
 *
 *   It's necessary for window name when user changes a focused window.
 *   (kAXTitleChangedNotification will not be sent when user changed focused window.)
 *
 *   Items that need to be updated:
 *     - title
 *
 *
 * kAXFocusedUIElementChangedNotification
 * --------------------------------------
 *
 *   A essential notification for ui element role.
 *   (kAXFocusedUIElementChangedNotification will also be triggered when user changed focused window.)
 *
 *   Items that need to be updated:
 *     - role
 *
 */

- (void) updateBundleIdentifier:(NSRunningApplication*)runningApplication
{
  @synchronized(self) {
    focusedUIElementInformation_[@"bundleIdentifier"] = [runningApplication bundleIdentifier];
  }
}

- (void) updateTitle
{
  @synchronized(self) {
    focusedUIElementInformation_[@"title"] = @"";

    AXUIElementRef element = [AXUtilities copyFocusedWindow];
    if (element) {
      NSString* title = [AXUtilities titleOfUIElement:element];
      if (title) {
        focusedUIElementInformation_[@"title"] = title;
      }
      CFRelease(element);
    }
  }
}

- (void) updateRole
{
  @synchronized(self) {
    focusedUIElementInformation_[@"role"] = @"";

    AXUIElementRef element = [AXUtilities copyFocusedUIElement];
    if (element) {
      NSString* role = [AXUtilities roleOfUIElement:element];
      if (role) {
        focusedUIElementInformation_[@"role"] = role;
      }
      CFRelease(element);
    }
  }
}

- (void) updateAll:(NSRunningApplication*)runningApplication
{
  [self updateBundleIdentifier:runningApplication];
  [self updateTitle];
  [self updateRole];
}

- (void) tellToServer
{
  NSLog(@"%@", focusedUIElementInformation_);
  [[self.client proxy] updateFocusedUIElementInformation:focusedUIElementInformation_];
}

static void observerCallback(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void* refcon)
{
  AppDelegate* self = (__bridge AppDelegate*)(refcon);
  if (! self) return;

  NSLog(@"observerCallback: %@", (__bridge NSString*)(notification));

  if (CFStringCompare(notification, kAXTitleChangedNotification, 0) == kCFCompareEqualTo ||
      CFStringCompare(notification, kAXFocusedWindowChangedNotification, 0) == kCFCompareEqualTo) {
    [self updateTitle];
    [self tellToServer];
  }
  if (CFStringCompare(notification, kAXFocusedUIElementChangedNotification, 0) == kCFCompareEqualTo) {
    [self updateRole];
    [self tellToServer];
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

  {
    NSArray* notifications = @[(__bridge NSString*)(kAXTitleChangedNotification),
                               (__bridge NSString*)(kAXFocusedWindowChangedNotification),
                               (__bridge NSString*)(kAXFocusedUIElementChangedNotification),
                             ];
    for (NSString* notification in notifications) {
      error = AXObserverAddNotification(observer_, application, (__bridge CFStringRef)(notification), (__bridge void*)self);
      if (error != kAXErrorSuccess) {
        NSLog(@"AXObserverAddNotification is failed: pid:%d error:%d", pid, error);
        goto finish;
      }
    }
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
  [self updateAll:runningApplication];
  [self tellToServer];
}

- (void) distributedObserver_kKeyRemap4MacBookServerDidLaunchNotification:(NSNotification*)notification
{
  [NSTask launchedTaskWithLaunchPath:[[NSBundle mainBundle] executablePath] arguments:@[]];
  [NSApp terminate:self];
}

- (void) timerFireMethod:(NSTimer*)timer
{
  dispatch_async(dispatch_get_main_queue(), ^{
    if (AXIsProcessTrusted()) {
      [[NSApplication sharedApplication] hide:self];

      if (! initialized_) {
        initialized_ = YES;

        NSRunningApplication* runningApplication = [[NSWorkspace sharedWorkspace] frontmostApplication];
        [self registerApplication:runningApplication];
        [self updateAll:runningApplication];
        [self tellToServer];
      }

    } else {
      initialized_ = NO;
    }
  });
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  focusedUIElementInformation_ = [NSMutableDictionary new];

  NSDictionary* options = @{ (__bridge NSString*)(kAXTrustedCheckOptionPrompt): @YES };
  AXIsProcessTrustedWithOptions((__bridge CFDictionaryRef)options);

  if (! AXIsProcessTrusted()) {
    if (! [[NSUserDefaults standardUserDefaults] boolForKey:kDoNotShowAXWarningMessage]) {
      [_window orderFront:self];
    }
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

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_kKeyRemap4MacBookServerDidLaunchNotification:)
                                                          name:kKeyRemap4MacBookServerDidLaunchNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];
}

@end
