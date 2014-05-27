#import "AXUtilities.h"
#import "AppDelegate.h"
#import "KeyRemap4MacBookKeys.h"
#import "PreferencesKeys.h"

// ==================================================
@interface AppDelegate ()
{
  BOOL initialized_;
  NSRunningApplication* runningApplication_;
  AXObserverRef observer_;
  AXUIElementRef applicationElement_;
  AXUIElementRef focusedWindowElement_;
  NSMutableDictionary* focusedUIElementInformation_;
  NSDictionary* previousSentInformation_;
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
 * - kAXFocusedUIElementChangedNotification
 * - kAXFocusedWindowChangedNotification
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
 *   We need to observe this notification in this case:
 *     The window name will be changed when we use Google Chrome and click a link.
 *     If the link does not open new tab, kAXFocusedUIElementChangedNotification is not triggered.
 *     So we observe the window name changes by kAXTitleChangedNotification.
 *
 *     Note:
 *     kAXTitleChangedNotification will not be called on Safari 7.0.4 in this case.
 *     It might be a bug of Safari 7.0.4.
 *
 *   Items that need to be updated:
 *     - title
 *
 *
 * kAXFocusedUIElementChangedNotification
 * --------------------------------------
 *
 *   A essential notification for ui element role.
 *
 *   On Safari, kAXTitleChangedNotification will not be called when user changed the current tab.
 *   (It might be a bug of Safari 7.0.4.)
 *   So we need to update title at kAXFocusedUIElementChangedNotification.
 *
 *   Items that need to be updated:
 *     - title
 *     - role
 *
 *   Note:
 *   kAXFocusedUIElementChangedNotification will also be triggered when user changed focused window.
 *   But kAXFocusedUIElementChangedNotification and kAXFocusedWindowChangedNotification is not unordered.
 *   Therefore, we need to update title at both notifications.
 *
 *
 * kAXFocusedWindowChangedNotification
 * -----------------------------------
 *
 *   We need to register kAXTitleChangedNotification to the focused window.
 *   (If register kAXTitleChangedNotification to application element,
 *   kAXTitleChangedNotification will not be triggered on Emacs.app.)
 *
 *   We need to observe this notification in order to refresh kAXTitleChangedNotification observer.
 *
 *   Items that need to be updated:
 *     - title
 *
 *   Note:
 *   See the note of kAXFocusedUIElementChangedNotification.
 *
 */

- (void) updateApplicationElement
{
  @synchronized(self) {
    // ----------------------------------------
    if (applicationElement_) {
      CFRelease(applicationElement_);
      applicationElement_ = NULL;
    }

    // ----------------------------------------
    if (! AXIsProcessTrusted()) return;

    // ----------------------------------------
    applicationElement_ = AXUIElementCreateApplication([runningApplication_ processIdentifier]);
    if (! applicationElement_) {
      NSLog(@"AXUIElementCreateApplication is failed. %@", runningApplication_);
    }
  }
}

- (void) updateFocusedWindowElement
{
  @synchronized(self) {
    // ----------------------------------------
    if (focusedWindowElement_) {
      CFRelease(focusedWindowElement_);
      focusedWindowElement_ = NULL;
    }

    // ----------------------------------------
    if (! AXIsProcessTrusted()) return;

    // ----------------------------------------
    AXError error = AXUIElementCopyAttributeValue(applicationElement_,
                                                  kAXFocusedWindowAttribute,
                                                  (CFTypeRef*)(&focusedWindowElement_));
    if (error != kAXErrorSuccess) {
      NSLog(@"updateFocusedWindowElement is failed: %@", runningApplication_);
    }
  }
}

- (void) updateBundleIdentifier
{
  @synchronized(self) {
    focusedUIElementInformation_[@"BundleIdentifier"] = [runningApplication_ bundleIdentifier];
  }
}

- (void) updateTitle
{
  @synchronized(self) {
    focusedUIElementInformation_[@"WindowName"] = @"";

    if (focusedWindowElement_) {
      NSString* title = [AXUtilities titleOfUIElement:focusedWindowElement_];
      if (title) {
        focusedUIElementInformation_[@"WindowName"] = title;
      }
    }
  }
}

- (void) updateRole
{
  @synchronized(self) {
    focusedUIElementInformation_[@"UIElementRole"] = @"";

    AXUIElementRef element = [AXUtilities copyFocusedUIElement];
    if (element) {
      NSString* role = [AXUtilities roleOfUIElement:element];
      if (role) {
        focusedUIElementInformation_[@"UIElementRole"] = role;
      }
      CFRelease(element);
    }
  }
}

- (void) tellToServer
{
  @synchronized(self) {
    // Send if the current information and the previous information are different.
    for (NSString* key in focusedUIElementInformation_) {
      if ([key isEqual:@"mtime"]) {
        continue;
      }

      if (! [focusedUIElementInformation_[key] isEqual:previousSentInformation_[key]]) {
        goto send;
      }
    }
#if 0
    NSLog(@"tellToServer skip");
#endif
    return;

  send:
    focusedUIElementInformation_[@"mtime"] = @((NSUInteger)([[NSDate date] timeIntervalSince1970] * 1000));
#if 0
    NSLog(@"%@", focusedUIElementInformation_);
#endif
    [[self.client proxy] updateFocusedUIElementInformation:focusedUIElementInformation_];

    previousSentInformation_ = [NSDictionary dictionaryWithDictionary:focusedUIElementInformation_];
  }
}

- (void) registerFocusNotifications
{
  @synchronized(self) {
    if (! observer_) return;
    if (! applicationElement_) return;

    NSArray* notifications = @[(__bridge NSString*)(kAXFocusedUIElementChangedNotification),
                               (__bridge NSString*)(kAXFocusedWindowChangedNotification),
                             ];
    for (NSString* notification in notifications) {
      AXError error = AXObserverAddNotification(observer_,
                                                applicationElement_,
                                                (__bridge CFStringRef)(notification),
                                                (__bridge void*)self);
      if (error != kAXErrorSuccess) {
        NSLog(@"AXObserverAddNotification is failed: error:%d %@", error, runningApplication_);
      }
    }
  }
}

- (void) registerTitleChangedNotification
{
  @synchronized(self) {
    if (! observer_) return;
    if (! focusedWindowElement_) return;

    AXError error = AXObserverAddNotification(observer_,
                                              focusedWindowElement_,
                                              kAXTitleChangedNotification,
                                              (__bridge void*)self);
    if (error != kAXErrorSuccess) {
      NSLog(@"AXObserverAddNotification is failed: error:%d %@", error, runningApplication_);
    }
  }
}

- (void) unregisterTitleChangedNotification
{
  @synchronized(self) {
    if (! observer_) return;
    if (! focusedWindowElement_) return;

    AXError error = AXObserverRemoveNotification(observer_,
                                                 focusedWindowElement_,
                                                 kAXTitleChangedNotification);
    if (error != kAXErrorSuccess) {
      if (error == kAXErrorInvalidUIElement) {
        // Ignore this error because it is expected error when focusedWindowElement_ is closed.
      } else {
        NSLog(@"AXObserverRemoveNotification is failed: error:%d %@", error, runningApplication_);
      }
    }
  }
}

static void observerCallback(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void* refcon)
{
#if 0
  NSLog(@"%@", (__bridge NSString*)(notification));
#endif

  AppDelegate* self = (__bridge AppDelegate*)(refcon);
  if (! self) return;

  if (CFStringCompare(notification, kAXTitleChangedNotification, 0) == kCFCompareEqualTo) {
    [self updateTitle];
    [self tellToServer];
  }
  if (CFStringCompare(notification, kAXFocusedUIElementChangedNotification, 0) == kCFCompareEqualTo) {
    [self updateTitle];
    [self updateRole];
    [self tellToServer];
  }
  if (CFStringCompare(notification, kAXFocusedWindowChangedNotification, 0) == kCFCompareEqualTo) {
    // ----------------------------------------
    // unregister notifications.
    [self unregisterTitleChangedNotification];

    // ----------------------------------------
    // update AX variables.
    [self updateFocusedWindowElement];

    // ----------------------------------------
    // register notifications.
    [self registerTitleChangedNotification];

    // ----------------------------------------
    [self updateTitle];
    [self tellToServer];
  }
}

- (void) updateObserver
{
  @synchronized(self) {
    // ----------------------------------------
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

    error = AXObserverCreate([runningApplication_ processIdentifier], observerCallback, &observer_);
    if (error != kAXErrorSuccess) {
      observer_ = NULL;
      NSLog(@"AXObserverCreate is failed. error:%d %@", error, runningApplication_);
      return;
    }

    CFRunLoopAddSource(CFRunLoopGetCurrent(),
                       AXObserverGetRunLoopSource(observer_),
                       kCFRunLoopDefaultMode);
  }
}

- (void) didActivateApplication:(NSRunningApplication*)runningApplication
{
  runningApplication_ = runningApplication;

  // ----------------------------------------
  // update AX variables.
  [self updateObserver];
  [self updateApplicationElement];
  [self updateFocusedWindowElement];

  // ----------------------------------------
  // register notifications.
  [self registerFocusNotifications];
  [self registerTitleChangedNotification];

  // ----------------------------------------
  // tell to server.
  [self updateBundleIdentifier];
  [self updateTitle];
  [self updateRole];
  [self tellToServer];
}

- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  NSRunningApplication* runningApplication = [notification userInfo][NSWorkspaceApplicationKey];
  [self didActivateApplication:runningApplication];
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
        [self didActivateApplication:runningApplication];
      }

    } else {
      initialized_ = NO;
    }
  });
}

#define kDescendantProcess @"org_pqrs_KeyRemap4MacBook_AXNotifier_DescendantProcess"

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSInteger isDescendantProcess = [[[NSProcessInfo processInfo] environment][kDescendantProcess] integerValue];
  setenv([kDescendantProcess UTF8String], "1", 1);

  // ------------------------------------------------------------
  focusedUIElementInformation_ = [NSMutableDictionary new];

  [_window setLevel:NSFloatingWindowLevel];

  if (! [[NSUserDefaults standardUserDefaults] boolForKey:kDoNotShowAXWarningMessage]) {
    if (! AXIsProcessTrusted()) {
      [_window orderFront:self];

      if (! isDescendantProcess) {
        NSDictionary* options = @{ (__bridge NSString*)(kAXTrustedCheckOptionPrompt): @YES };
        AXIsProcessTrustedWithOptions((__bridge CFDictionaryRef)options);
      }
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
