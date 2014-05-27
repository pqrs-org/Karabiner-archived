#import "AXUtilities.h"
#import "AppDelegate.h"
#import "FrontmostWindow.h"
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
 * - NSWorkspaceActiveSpaceDidChangeNotification
 * - kAXTitleChangedNotification
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
 * NSWorkspaceActiveSpaceDidChangeNotification
 * -------------------------------------------
 *
 *   We are using FrontmostWindow to get window name in some cases. (eg. Microsoft Remote Desktop.)
 *
 *   If user switched to other apps which are not in the current space,
 *   FrontmostWindow failed to get properly window name.
 *   Because FrontmostWindow collects shown windows on screen and
 *   there is no other apps's window when it was called. (They are in other spaces.)
 *
 *   Therefore, we need to observe NSWorkspaceActiveSpaceDidChangeNotification for this case.
 *
 *   Items that need to be updated:
 *     - title
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
 *   kAXFocusedUIElementChangedNotification will also be triggered when user changed focused window.
 *   Therefore, we don't need to observe kAXFocusedWindowChangedNotification.
 *
 *   Items that need to be updated:
 *     - title
 *     - role
 *
 */

- (void) updateBundleIdentifier:(NSRunningApplication*)runningApplication
{
  @synchronized(self) {
    focusedUIElementInformation_[@"BundleIdentifier"] = [runningApplication bundleIdentifier];
  }
}

- (void) updateTitle
{
  @synchronized(self) {
    focusedUIElementInformation_[@"WindowName"] = @"";

    AXUIElementRef element = [AXUtilities copyFocusedWindow];
    if (element) {
      NSString* title = [AXUtilities titleOfUIElement:element];
      if (title) {
        focusedUIElementInformation_[@"WindowName"] = title;
      }
      CFRelease(element);

    } else {
      // Getting kAXFocusedUIElementAttribute will be failed in some cases. (eg. Microsoft Remote Desktop.)
      // In this case, we use to FrontmostWindow to get the window name.
      // (We do not use FrontmostWindow always because it is a relatively expensive operation.)
      //
      // Note:
      // FrontmostWindow returns wrong window name
      // when Finder windows are opened and the current focus is desktop icon.
      // (The window name should be empty but FrontmostWindow return the frontmost window's name.)
      // So, we should not call FrontmostWindow everytime.
      //
      FrontmostWindow* frontmostWindow = [FrontmostWindow new];
      if (frontmostWindow.windowName) {
        focusedUIElementInformation_[@"WindowName"] = frontmostWindow.windowName;
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

- (void) updateAll:(NSRunningApplication*)runningApplication
{
  [self updateBundleIdentifier:runningApplication];
  [self updateTitle];
  [self updateRole];
}

- (void) tellToServer
{
  focusedUIElementInformation_[@"mtime"] = @((NSUInteger)([[NSDate date] timeIntervalSince1970] * 1000));
#if 0
  NSLog(@"%@", focusedUIElementInformation_);
#endif
  [[self.client proxy] updateFocusedUIElementInformation:focusedUIElementInformation_];
}

static void observerCallback(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void* refcon)
{
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

- (void) observer_NSWorkspaceActiveSpaceDidChangeNotification:(NSNotification*)notification
{
  [self updateTitle];
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

#define kDescendantProcess @"org_pqrs_KeyRemap4MacBook_AXNotifier_DescendantProcess"

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSInteger isDescendantProcess = [[[NSProcessInfo processInfo] environment][kDescendantProcess] integerValue];
  setenv([kDescendantProcess UTF8String], "1", 1);

  // ------------------------------------------------------------
  focusedUIElementInformation_ = [NSMutableDictionary new];

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

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceActiveSpaceDidChangeNotification:)
                                                             name:NSWorkspaceActiveSpaceDidChangeNotification
                                                           object:nil];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_kKeyRemap4MacBookServerDidLaunchNotification:)
                                                          name:kKeyRemap4MacBookServerDidLaunchNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];
}

@end
