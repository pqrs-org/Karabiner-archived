#import <Carbon/Carbon.h>
#import "AppDelegate.h"
#import "ClientForKernelspace.h"
#import "FrontmostWindow.h"
#import "KeyRemap4MacBookKeys.h"
#import "NotificationKeys.h"
#import "PreferencesController.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "Relauncher.h"
#import "ServerForUserspace.h"
#import "StatusBar.h"
#import "StatusWindow.h"
#import "Updater.h"
#import "WorkSpaceData.h"
#include <stdlib.h>

@interface AppDelegate ()
{
  NSMutableDictionary* applicationInformation_;
  NSMutableDictionary* inputSourceInformation_;

  // for IONotification
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;

  struct BridgeWorkSpaceData bridgeworkspacedata_;
}
@end

@implementation AppDelegate

@synthesize clientForKernelspace;

// ----------------------------------------
- (void) send_workspacedata_to_kext
{
  [clientForKernelspace send_workspacedata_to_kext:&bridgeworkspacedata_];
}

- (void) updateApplicationInformation
{
  NSString* name = [WorkSpaceData getActiveApplicationName];
  if (! name) return;

  // We ignore our investigation application.
  if ([name isEqualToString:@"org.pqrs.KeyRemap4MacBook.EventViewer"]) return;

  bridgeworkspacedata_.applicationtype = [workSpaceData_ getApplicationType:name];

  FrontmostWindow* frontmostWindow = [FrontmostWindow new];
  if (frontmostWindow.windowName) {
    bridgeworkspacedata_.windowname = [workSpaceData_ getWindowName:frontmostWindow.windowName];
  }

  [self send_workspacedata_to_kext];

  @synchronized(self) {
    applicationInformation_ = [NSMutableDictionary new];
    applicationInformation_[@"name"] = name;
    if (frontmostWindow.windowName) {
      applicationInformation_[@"windowName"] = frontmostWindow.windowName;
    }
  }

  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKeyRemap4MacBookApplicationChangedNotification
                                                                 object:nil];
}

// ------------------------------------------------------------
- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self updateApplicationInformation];
  });
}

- (void) observer_NSWorkspaceActiveSpaceDidChangeNotification:(NSNotification*)notification
{
  // We need to observe this NSWorkspaceActiveSpaceDidChangeNotification.
  //
  // Because NSWorkspaceDidActivateApplicationNotification does not provide properly windowName when
  // NSWorkspaceDidActivateApplicationNotification is called with the space switching.
  //
  // NSWorkspaceDidActivateApplicationNotification will be called before space switching is completed.
  // Then, the windowName will be empty because there is no window at screen.

  dispatch_async(dispatch_get_main_queue(), ^{
    [self updateApplicationInformation];
  });
}

- (void) distributedObserver_kKeyRemap4MacBookAXTitleChangedNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self updateApplicationInformation];
  });
}

- (void) distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [WorkSpaceData refreshEnabledInputSources];
  });
}

- (void) distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    InputSource* inputSource = [WorkSpaceData getCurrentInputSource];
    [workSpaceData_ getInputSourceID:inputSource
                  output_inputSource:(&(bridgeworkspacedata_.inputsource))
            output_inputSourceDetail:(&(bridgeworkspacedata_.inputsourcedetail))];
    [self send_workspacedata_to_kext];

    @synchronized(self) {
      inputSourceInformation_ = [NSMutableDictionary new];
      if ([inputSource languagecode]) {
        inputSourceInformation_[@"languageCode"] = [inputSource languagecode];
      }
      if ([inputSource inputSourceID]) {
        inputSourceInformation_[@"inputSourceID"] = [inputSource inputSourceID];
      }
      if ([inputSource inputModeID]) {
        inputSourceInformation_[@"inputModeID"] = [inputSource inputModeID];
      }
    }

    [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKeyRemap4MacBookInputSourceChangedNotification
                                                                   object:nil];
  });
}

- (void) observer_ConfigXMLReloaded:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    // If <appdef> or <inputsourcedef> is updated,
    // the following values might be changed.
    // Therefore, we need to resend values to kext.
    //
    // - bridgeworkspacedata_.applicationtype
    // - bridgeworkspacedata_.inputsource
    // - bridgeworkspacedata_.inputsourcedetail

    [self observer_NSWorkspaceDidActivateApplicationNotification:nil];
    [self distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:nil];
    [self distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:nil];
  });
}

// ------------------------------------------------------------
- (void) callClearNotSave
{
  if (! [preferencesManager_ value:@"general.keep_notsave_at_wake"]) {
    // disable notsave.* in order to disable "Browsing Mode" and
    // other modes which overwrite some keys
    // because these modes annoy password input.
    [preferencesManager_ clearNotSave];
  }
}

- (void) observer_NSWorkspaceDidWakeNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"observer_NSWorkspaceDidWakeNotification");
    [self callClearNotSave];
  });
}

- (void) observer_NSWorkspaceScreensDidWakeNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"observer_NSWorkspaceScreensDidWakeNotification");
    [self callClearNotSave];
  });
}

- (void) registerWakeNotification
{
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidWakeNotification:)
                                                             name:NSWorkspaceDidWakeNotification
                                                           object:nil];
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceScreensDidWakeNotification:)
                                                             name:NSWorkspaceScreensDidWakeNotification
                                                           object:nil];
}

- (void) unregisterWakeNotification
{
  [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:self
                                                                name:NSWorkspaceDidWakeNotification
                                                              object:nil];
  [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:self
                                                                name:NSWorkspaceScreensDidWakeNotification
                                                              object:nil];
}

// ------------------------------------------------------------
static void observer_IONotification(void* refcon, io_iterator_t iterator)
{
  dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"observer_IONotification");

    AppDelegate* self = (__bridge AppDelegate*)(refcon);
    if (! self) {
      NSLog(@"[ERROR] observer_IONotification refcon == nil\n");
      return;
    }

    for (;;) {
      io_object_t obj = IOIteratorNext(iterator);
      if (! obj) break;

      IOObjectRelease(obj);
    }
    // Do not release iterator.

    // = Documentation of IOKit =
    // - Introduction to Accessing Hardware From Applications
    //   - Finding and Accessing Devices
    //
    // In the case of IOServiceAddMatchingNotification, make sure you release the iterator only if you’re also ready to stop receiving notifications:
    // When you release the iterator you receive from IOServiceAddMatchingNotification, you also disable the notification.

    // ------------------------------------------------------------
    [[self clientForKernelspace] refresh_connection_with_retry];
    [self send_workspacedata_to_kext];
  });
}

- (void) unregisterIONotification
{
  if (notifyport_) {
    if (loopsource_) {
      CFRunLoopSourceInvalidate(loopsource_);
      loopsource_ = nil;
    }
    IONotificationPortDestroy(notifyport_);
    notifyport_ = nil;
  }
}

- (void) registerIONotification
{
  [self unregisterIONotification];

  notifyport_ = IONotificationPortCreate(kIOMasterPortDefault);
  if (! notifyport_) {
    NSLog(@"[ERROR] IONotificationPortCreate failed\n");
    return;
  }

  // ----------------------------------------------------------------------
  io_iterator_t it;
  kern_return_t kernResult;

  kernResult = IOServiceAddMatchingNotification(notifyport_,
                                                kIOMatchedNotification,
                                                IOServiceNameMatching("org_pqrs_driver_KeyRemap4MacBook"),
                                                &observer_IONotification,
                                                (__bridge void*)(self),
                                                &it);
  if (kernResult != kIOReturnSuccess) {
    NSLog(@"[ERROR] IOServiceAddMatchingNotification failed");
    return;
  }
  observer_IONotification((__bridge void*)(self), it);

  // ----------------------------------------------------------------------
  loopsource_ = IONotificationPortGetRunLoopSource(notifyport_);
  if (! loopsource_) {
    NSLog(@"[ERROR] IONotificationPortGetRunLoopSource failed");
    return;
  }
  CFRunLoopAddSource(CFRunLoopGetCurrent(), loopsource_, kCFRunLoopDefaultMode);
}

// ------------------------------------------------------------
- (void) observer_NSWorkspaceSessionDidBecomeActiveNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"observer_NSWorkspaceSessionDidBecomeActiveNotification");

    [statusWindow_ resetStatusMessage];

    [self registerIONotification];
    [self registerWakeNotification];
  });
}

- (void) observer_NSWorkspaceSessionDidResignActiveNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"observer_NSWorkspaceSessionDidResignActiveNotification");

    [statusWindow_ resetStatusMessage];

    [self unregisterIONotification];
    [self unregisterWakeNotification];
    [clientForKernelspace disconnect_from_kext];
  });
}

// ------------------------------------------------------------
#define kDescendantProcess @"org_pqrs_KeyRemap4MacBook_DescendantProcess"

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSInteger isDescendantProcess = [[[NSProcessInfo processInfo] environment][kDescendantProcess] integerValue];
  setenv([kDescendantProcess UTF8String], "1", 1);

  // ------------------------------------------------------------
  system("/Applications/KeyRemap4MacBook.app/Contents/Library/bin/kextload load");

  // ------------------------------------------------------------
  BOOL fromLaunchAgents = NO;

  for (NSString* argument in [[NSProcessInfo processInfo] arguments]) {
    if ([argument isEqualToString:@"--fromLaunchAgents"]) {
      fromLaunchAgents = YES;
    }
  }

  if (fromLaunchAgents) {
    // ------------------------------------------------------------
    // Remove old pkg files and finish_installation.app in
    // "~/Library/Application Support/KeyRemap4MacBook/.Sparkle".
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSString* sparkle = paths[0];
    if (sparkle) {
      sparkle = [sparkle stringByAppendingPathComponent:@"KeyRemap4MacBook"];
      sparkle = [sparkle stringByAppendingPathComponent:@".Sparkle"];

      NSFileManager* fm = [NSFileManager defaultManager];
      if ([fm fileExistsAtPath:sparkle]) {
        [fm removeItemAtPath:sparkle error:nil];
      }
    }

    // ------------------------------------------------------------
    if ([[NSUserDefaults standardUserDefaults] boolForKey:kIsQuitByHand]) {
      [NSApp terminate:nil];
    }
  }
  [PreferencesManager setIsQuitByHand:@NO];

  // ------------------------------------------------------------
  if (! [serverForUserspace_ register]) {
    // Relaunch when register is failed.
    NSLog(@"[ServerForUserspace register] is failed. Restarting process.");
    [NSThread sleepForTimeInterval:2];
    [Relauncher relaunch];
  }
  [Relauncher resetRelaunchedCount];

  // Wait until other apps connect to me.
  [NSThread sleepForTimeInterval:1];

  [preferencesManager_ load];

  [self registerIONotification];
  [self registerWakeNotification];

  [statusWindow_ setupStatusWindow];
  [statusbar_ refresh];
  [xmlCompiler_ reload];

  // ------------------------------------------------------------
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceActiveSpaceDidChangeNotification:)
                                                             name:NSWorkspaceActiveSpaceDidChangeNotification
                                                           object:nil];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_kKeyRemap4MacBookAXTitleChangedNotification:)
                                                          name:(NSString*)(kKeyRemap4MacBookAXTitleChangedNotification)
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];

  // We need to speficy NSNotificationSuspensionBehaviorDeliverImmediately for NSDistributedNotificationCenter
  // because kTISNotify* will be dropped sometimes without this.
  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:)
                                                          name:(NSString*)(kTISNotifyEnabledKeyboardInputSourcesChanged)
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:)
                                                          name:(NSString*)(kTISNotifySelectedKeyboardInputSourceChanged)
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];

  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(observer_ConfigXMLReloaded:)
                                               name:kConfigXMLReloadedNotification
                                             object:nil];

  // ------------------------------
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceSessionDidBecomeActiveNotification:)
                                                             name:NSWorkspaceSessionDidBecomeActiveNotification
                                                           object:nil];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceSessionDidResignActiveNotification:)
                                                             name:NSWorkspaceSessionDidResignActiveNotification
                                                           object:nil];

  // ------------------------------------------------------------
  [self observer_NSWorkspaceDidActivateApplicationNotification:nil];
  [self distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:nil];
  [self distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:nil];

  [updater_ checkForUpdatesInBackground:nil];

  // ------------------------------------------------------------
  // Launch AXNotifier if needed.
  if ([[NSUserDefaults standardUserDefaults] boolForKey:kLaunchAXNotifierAutomatically]) {
    [self launchAXNotifier:nil];
  }

  // ------------------------------------------------------------
  // Send kKeyRemap4MacBookServerDidLaunchNotification after launching AXNotifier.
  //
  // AXNotifier will be relaunched by kKeyRemap4MacBookServerDidLaunchNotification.
  // If we send the notification before launching AXNotifier,
  // two AXNotifier processes will be launched when AXNotifier is already running.
  //
  // * relaunched AXNotifier.
  // * AXNotifier launched by launchAXNotifier.

  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKeyRemap4MacBookServerDidLaunchNotification
                                                                 object:nil];

  // ------------------------------------------------------------
  // Open Preferences if KeyRemap4MacBook was launched by hand.
  if (! fromLaunchAgents &&
      ! isDescendantProcess) {
    [preferencesController_ show];
  }
}

- (BOOL) applicationShouldHandleReopen:(NSApplication*)theApplication hasVisibleWindows:(BOOL)flag
{
  [preferencesController_ show];
  return YES;
}

- (void) dealloc
{
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

// ------------------------------------------------------------
- (NSDictionary*) getApplicationInformation
{
  @synchronized(self) {
    return applicationInformation_;
  }
}
- (NSDictionary*) getInputSourceInformation
{
  @synchronized(self) {
    return inputSourceInformation_;
  }
}

// ------------------------------------------------------------
- (IBAction) launchEventViewer:(id)sender
{
  NSString* path = @"/Applications/KeyRemap4MacBook.app/Contents/Applications/EventViewer.app";
  [[NSWorkspace sharedWorkspace] launchApplication:path];
}

- (IBAction) launchMultiTouchExtension:(id)sender
{
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Applications/KeyRemap4MacBook.app/Contents/Applications/KeyRemap4MacBook_multitouchextension.app"];
}

- (IBAction) launchAXNotifier:(id)sender
{
  if (sender) {
    // Launch from button.
    // Check existing processes.
    NSArray* apps = [NSRunningApplication runningApplicationsWithBundleIdentifier:@"org.pqrs.KeyRemap4MacBook-AXNotifier"];
    if ([apps count] > 0) {
      [[NSAlert alertWithMessageText:@"AXNotifier is already running."
                       defaultButton:nil
                     alternateButton:nil
                         otherButton:nil
           informativeTextWithFormat:@""] runModal];
      return;
    }
  }

  NSString* path = @"/Applications/KeyRemap4MacBook.app/Contents/Applications/KeyRemap4MacBook_AXNotifier.app";
  [[NSWorkspace sharedWorkspace] launchApplication:path];
}

- (IBAction) launchUninstaller:(id)sender
{
  system("/Applications/KeyRemap4MacBook.app/Contents/Library/extra/launchUninstaller.sh");
}

- (IBAction) openPreferences:(id)sender
{
  [preferencesController_ show];
}

- (IBAction) openPrivateXML:(id)sender
{
  // Open a directory which contains private.xml.
  NSString* path = [XMLCompiler get_private_xml_path];
  if ([path length] > 0) {
    [[NSWorkspace sharedWorkspace] openFile:[path stringByDeletingLastPathComponent]];
  }
}

- (IBAction) quit:(id)sender
{
  NSAlert* alert = [NSAlert alertWithMessageText:@"Quit KeyRemap4MacBook?"
                                   defaultButton:@"Quit"
                                 alternateButton:@"Cancel"
                                     otherButton:nil
                       informativeTextWithFormat:@"Are you sure you want to quit KeyRemap4MacBook?"];
  if ([alert runModal] != NSAlertDefaultReturn) return;

  [PreferencesManager setIsQuitByHand:@YES];
  [NSApp terminate:nil];
}

@end
