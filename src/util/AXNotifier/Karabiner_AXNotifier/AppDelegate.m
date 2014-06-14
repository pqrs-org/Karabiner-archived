#import "AXUtilities.h"
#import "AppDelegate.h"
#import "KarabinerKeys.h"
#import "KarabinerUtilities.h"
#import "MigrationUtilities.h"
#import "PreferencesKeys.h"

// ==================================================
@interface AppDelegate ()
{
  BOOL initialized_;
  BOOL observerRegistered_;
  int observerRegisterRetryCounter_;

  NSRunningApplication* runningApplication_;
  AXObserverRef observer_;
  AXUIElementRef applicationElement_;
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

- (BOOL) updateApplicationElement
{
  // ----------------------------------------
  if (applicationElement_) {
    CFRelease(applicationElement_);
    applicationElement_ = NULL;
  }

  // ----------------------------------------
  if (! AXIsProcessTrusted()) return YES;

  // ----------------------------------------
  applicationElement_ = AXUIElementCreateApplication([runningApplication_ processIdentifier]);
  if (! applicationElement_) {
    NSLog(@"AXUIElementCreateApplication is failed. %@", runningApplication_);
    return NO;
  }

  return YES;
}

- (void) updateBundleIdentifier
{
  focusedUIElementInformation_[@"BundleIdentifier"] = [KarabinerUtilities bundleIdentifier:runningApplication_];
}

- (void) updateTitle
{
  if (! applicationElement_) return;

  focusedUIElementInformation_[@"WindowName"] = @"";

  // Do not cache focusedWindowElement.
  // We need to get new focusedWindowElement because
  // getting title will be failed with cached focusedWindowElement on Finder.app.

  AXUIElementRef focusedWindowElement = [AXUtilities copyFocusedWindow:applicationElement_];
  if (focusedWindowElement) {
    NSString* title = [AXUtilities titleOfUIElement:focusedWindowElement];
    if (title) {
      focusedUIElementInformation_[@"WindowName"] = title;
    }
    CFRelease(focusedWindowElement);
  }
}

- (void) updateRole:(AXUIElementRef)element
{
  focusedUIElementInformation_[@"UIElementRole"] = @"";

  if (element) {
    CFRetain(element);
  } else {
    element = [AXUtilities copyFocusedUIElement];
  }
  if (element) {
    NSString* role = [AXUtilities roleOfUIElement:element];
    if (role) {
      focusedUIElementInformation_[@"UIElementRole"] = role;
    }
    CFRelease(element);
  }
}

- (void) tellToServer
{
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
  @try {
    [[self.client proxy] updateFocusedUIElementInformation:focusedUIElementInformation_];
    previousSentInformation_ = [NSDictionary dictionaryWithDictionary:focusedUIElementInformation_];
  } @catch (NSException* exception) {
    NSLog(@"%@", exception);
    previousSentInformation_ = nil;
  }
}

- (BOOL) observeAXNotification:(AXUIElementRef)element notification:(CFStringRef)notification add:(BOOL)add
{
  if (! observer_) return YES;

  if (add) {
    AXError error = AXObserverAddNotification(observer_,
                                              element,
                                              notification,
                                              (__bridge void*)self);
    if (error != kAXErrorSuccess) {
      if (error == kAXErrorNotificationUnsupported ||
          error == kAXErrorNotificationAlreadyRegistered) {
        // We ignore this error.
        return YES;
      }
      NSLog(@"AXObserverAddNotification is failed: error:%d %@", error, runningApplication_);
      return NO;
    }

  } else {
    AXError error = AXObserverRemoveNotification(observer_,
                                                 element,
                                                 notification);
    if (error != kAXErrorSuccess) {
      // Note: Ignore kAXErrorInvalidUIElement because it is expected error when focused window is closed.
      if (error == kAXErrorInvalidUIElement ||
          error == kAXErrorNotificationNotRegistered) {
        // We ignore this error.
        return YES;
      }
      NSLog(@"AXObserverRemoveNotification is failed: error:%d %@", error, runningApplication_);
      return NO;
    }
  }

  return YES;
}

- (BOOL) registerFocusNotifications:(BOOL)add
{
  if (! observer_) return YES;
  if (! applicationElement_) return YES;

  if (! [self observeAXNotification:applicationElement_ notification:kAXFocusedUIElementChangedNotification add:add] ||
      ! [self observeAXNotification:applicationElement_ notification:kAXFocusedWindowChangedNotification add:add]) {
    return NO;
  }

  return YES;
}

- (BOOL) registerTitleChangedNotification:(BOOL)add
{
  if (! observer_) return YES;
  if (! applicationElement_) return YES;

  AXUIElementRef focusedWindowElement = [AXUtilities copyFocusedWindow:applicationElement_];
  if (! focusedWindowElement) return NO;

  if (! [self observeAXNotification:focusedWindowElement notification:kAXTitleChangedNotification add:add]) {
    return NO;
  }

  return YES;
}

static void observerCallback(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void* refcon)
{
#if 0
  NSLog(@"%@", (__bridge NSString*)(notification));
#endif

  AppDelegate* self = (__bridge AppDelegate*)(refcon);
  if (! self) return;

  @synchronized(self) {
    if (CFStringCompare(notification, kAXTitleChangedNotification, 0) == kCFCompareEqualTo) {
      [self updateTitle];
      [self tellToServer];
    }
    if (CFStringCompare(notification, kAXFocusedUIElementChangedNotification, 0) == kCFCompareEqualTo) {
      [self updateTitle];
      [self updateRole:element];
      [self tellToServer];
    }
    if (CFStringCompare(notification, kAXFocusedWindowChangedNotification, 0) == kCFCompareEqualTo) {
      // ----------------------------------------
      // refresh notification.
      [self registerTitleChangedNotification:NO];
      [self registerTitleChangedNotification:YES];

      // ----------------------------------------
      [self updateTitle];
      [self tellToServer];
    }
  }
}

- (BOOL) updateObserver
{
  // ----------------------------------------
  if (observer_) {
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(),
                          AXObserverGetRunLoopSource(observer_),
                          kCFRunLoopDefaultMode);

    CFRelease(observer_);
    observer_ = NULL;
  }

  // ----------------------------------------
  if (! AXIsProcessTrusted()) return YES;

  // ----------------------------------------
  AXError error = kAXErrorSuccess;

  error = AXObserverCreate([runningApplication_ processIdentifier], observerCallback, &observer_);
  if (error != kAXErrorSuccess) {
    observer_ = NULL;
    NSLog(@"AXObserverCreate is failed. error:%d %@", error, runningApplication_);
    return NO;
  }

  CFRunLoopAddSource(CFRunLoopGetCurrent(),
                     AXObserverGetRunLoopSource(observer_),
                     kCFRunLoopDefaultMode);

  return YES;
}

- (void) didActivateApplication:(NSRunningApplication*)runningApplication
{
  runningApplication_ = runningApplication;

  observerRegistered_ = NO;

  // ----------------------------------------
  // unregister notifications. (Do not check error)
  [self registerFocusNotifications:NO];
  [self registerTitleChangedNotification:NO];

  // ----------------------------------------
  // update AX variables.
  if (! [self updateObserver]) goto tell_to_server;
  if (! [self updateApplicationElement]) goto tell_to_server;

  // ----------------------------------------
  // register notifications.
  if (! [self registerFocusNotifications:YES]) goto tell_to_server;
  if (! [self registerTitleChangedNotification:YES]) goto tell_to_server;

  observerRegistered_ = YES;

tell_to_server:
  // ----------------------------------------
  // tell to server.
  [self updateBundleIdentifier];
  [self updateTitle];
  [self updateRole:NULL];
  [self tellToServer];
}

- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      NSRunningApplication* runningApplication = [notification userInfo][NSWorkspaceApplicationKey];
      [self didActivateApplication:runningApplication];
    }
  });
}

- (void) distributedObserver_kKarabinerServerDidLaunchNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      [NSTask launchedTaskWithLaunchPath:[[NSBundle mainBundle] executablePath] arguments:@[]];
      [NSApp terminate:self];
    }
  });
}

- (void) timerFireMethod:(NSTimer*)timer
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      if (AXIsProcessTrusted()) {
        if (! [[NSApplication sharedApplication] isHidden]) {
          [[NSApplication sharedApplication] hide:self];
        }

        if (! initialized_) {
          initialized_ = YES;

          NSRunningApplication* runningApplication = [[NSWorkspace sharedWorkspace] frontmostApplication];
          [self didActivateApplication:runningApplication];
        }

        // AXObserverAddNotification might be failed when just application launched.
        // So, we try to re-register notification by timer.
        if (observerRegistered_) {
          if (observerRegisterRetryCounter_ > 0) {
            NSLog(@"notifications registration has been finished.");
          }
          observerRegisterRetryCounter_ = 0;
        } else {
          ++observerRegisterRetryCounter_;
          NSRunningApplication* runningApplication = [[NSWorkspace sharedWorkspace] frontmostApplication];
          NSLog(@"register notifications: retry counter:%d %@", observerRegisterRetryCounter_, runningApplication);
          [self didActivateApplication:runningApplication];
        }

      } else {
        initialized_ = NO;
      }
    }
  });
}

#define kDescendantProcess @"org_pqrs_Karabiner_AXNotifier_DescendantProcess"

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSInteger isDescendantProcess = [[[NSProcessInfo processInfo] environment][kDescendantProcess] integerValue];
  setenv([kDescendantProcess UTF8String], "1", 1);

  // ------------------------------------------------------------
  [MigrationUtilities migrate:@[@"org.pqrs.KeyRemap4MacBook.AXNotifier"]
       oldApplicationSupports:@[]
                     oldPaths:@[@"/Applications/KeyRemap4MacBook.app/Contents/Applications/KeyRemap4MacBook_AXNotifier.app"]];

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

  {
    NSRunningApplication* runningApplication = [[NSWorkspace sharedWorkspace] frontmostApplication];
    [self didActivateApplication:runningApplication];
  }

  [NSTimer scheduledTimerWithTimeInterval:0.5
                                   target:self
                                 selector:@selector(timerFireMethod:)
                                 userInfo:nil
                                  repeats:YES];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_kKarabinerServerDidLaunchNotification:)
                                                          name:kKarabinerServerDidLaunchNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];
}

@end
