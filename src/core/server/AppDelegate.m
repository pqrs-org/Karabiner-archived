#import <Carbon/Carbon.h>
#import "AppDelegate.h"
#import "ClientForKernelspace.h"
#import "KarabinerKeys.h"
#import "MigrationUtilities.h"
#import "NotificationKeys.h"
#import "PreferencesController.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "Relauncher.h"
#import "ServerForUserspace.h"
#import "SessionObserver.h"
#import "StartAtLoginUtilities.h"
#import "StatusBar.h"
#import "StatusMessageManager.h"
#import "Updater.h"
#import "WorkSpaceData.h"

#include <stdlib.h>

@interface AppDelegate () {
  NSDictionary* focusedUIElementInformation_;
  NSMutableDictionary* inputSourceInformation_;

  // for IONotification
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;

  dispatch_queue_t axnotifierManagerQueue_;

  NSArray* workspaceAppIds_;
  NSArray* workspaceWindowNameIds_;
  NSNumber* workspaceUIElementRoleId_;
  NSArray* workspaceInputSourceIds_;

  SessionObserver* sessionObserver_;
}
@end

@implementation AppDelegate

@synthesize clientForKernelspace;

// ----------------------------------------
- (void)send_workspacedata_to_kext {
  NSMutableArray* workspaceIds = [NSMutableArray new];
  // Add dummy item to ensure send data.
  [workspaceIds addObject:@(BRIDGE_WORKSPACETYPE_NONE)];
  [workspaceIds addObject:@0];

  for (NSNumber* n in workspaceAppIds_) {
    [workspaceIds addObject:@(BRIDGE_WORKSPACETYPE_APP_ID)];
    [workspaceIds addObject:n];
  }

  for (NSNumber* n in workspaceInputSourceIds_) {
    [workspaceIds addObject:@(BRIDGE_WORKSPACETYPE_INPUT_SOURCE_ID)];
    [workspaceIds addObject:n];
  }

  if (workspaceUIElementRoleId_) {
    [workspaceIds addObject:@(BRIDGE_WORKSPACETYPE_UI_ELEMENT_ROLE_ID)];
    [workspaceIds addObject:workspaceUIElementRoleId_];
  }

  for (NSNumber* n in workspaceWindowNameIds_) {
    [workspaceIds addObject:@(BRIDGE_WORKSPACETYPE_WINDOW_NAME_ID)];
    [workspaceIds addObject:n];
  }

  [clientForKernelspace send_workspacedata_to_kext:workspaceIds];
}

// ------------------------------------------------------------
- (void)distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [WorkSpaceData refreshEnabledInputSources];
  });
}

- (void)distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    InputSource* inputSource = [WorkSpaceData getCurrentInputSource];
    if (!inputSource) {
      workspaceInputSourceIds_ = nil;
    } else {
      workspaceInputSourceIds_ = [xmlCompiler_ inputsourceids:inputSource.languagecode
                                                inputSourceID:inputSource.inputSourceID
                                                  inputModeID:inputSource.inputModeID];
    }
    [self send_workspacedata_to_kext];

    @synchronized(self) {
      inputSourceInformation_ = [NSMutableDictionary new];
      inputSourceInformation_[@"mtime"] = @((NSUInteger)([[NSDate date] timeIntervalSince1970] * 1000));

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
  });
}

- (void)observer_ConfigXMLReloaded:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    // If <appdef> or <inputsourcedef> is updated,
    // the following values might be changed.
    // Therefore, we need to resend values to kext.
    //
    // - workspaceAppIds_
    // - workspaceWindowNameIds_
    // - workspaceUIElementRoleId_
    // - workspaceInputSourceIds_

    [self updateFocusedUIElementInformation:nil];
    [self distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:nil];
    [self distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:nil];
  });
}

// ------------------------------------------------------------
- (void)callClearNotSave {
  if (![preferencesManager_ value:@"general.keep_notsave_at_wake"]) {
    // disable notsave.* in order to disable "Browsing Mode" and
    // other modes which overwrite some keys
    // because these modes annoy password input.
    [preferencesManager_ clearNotSave];
  }
}

- (void)observer_NSWorkspaceDidWakeNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"observer_NSWorkspaceDidWakeNotification");
    [self callClearNotSave];
  });
}

- (void)observer_NSWorkspaceScreensDidWakeNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"observer_NSWorkspaceScreensDidWakeNotification");
    [self callClearNotSave];
  });
}

- (void)registerWakeNotification {
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidWakeNotification:)
                                                             name:NSWorkspaceDidWakeNotification
                                                           object:nil];
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceScreensDidWakeNotification:)
                                                             name:NSWorkspaceScreensDidWakeNotification
                                                           object:nil];
}

- (void)unregisterWakeNotification {
  [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:self
                                                                name:NSWorkspaceDidWakeNotification
                                                              object:nil];
  [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:self
                                                                name:NSWorkspaceScreensDidWakeNotification
                                                              object:nil];
}

// ------------------------------------------------------------
static void observer_IONotification(void* refcon, io_iterator_t iterator) {
  dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"observer_IONotification");

    AppDelegate* self = (__bridge AppDelegate*)(refcon);
    if (!self) {
      NSLog(@"[ERROR] observer_IONotification refcon == nil\n");
      return;
    }

    for (;;) {
      io_object_t obj = IOIteratorNext(iterator);
      if (!obj) break;

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

- (void)unregisterIONotification {
  if (notifyport_) {
    if (loopsource_) {
      CFRunLoopSourceInvalidate(loopsource_);
      loopsource_ = nil;
    }
    IONotificationPortDestroy(notifyport_);
    notifyport_ = nil;
  }
}

- (void)registerIONotification {
  [self unregisterIONotification];

  notifyport_ = IONotificationPortCreate(kIOMasterPortDefault);
  if (!notifyport_) {
    NSLog(@"[ERROR] IONotificationPortCreate failed\n");
    return;
  }

  // ----------------------------------------------------------------------
  io_iterator_t it;
  kern_return_t kernResult;

  kernResult = IOServiceAddMatchingNotification(notifyport_,
                                                kIOMatchedNotification,
                                                IOServiceNameMatching("org_pqrs_driver_Karabiner"),
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
  if (!loopsource_) {
    NSLog(@"[ERROR] IONotificationPortGetRunLoopSource failed");
    return;
  }
  CFRunLoopAddSource(CFRunLoopGetCurrent(), loopsource_, kCFRunLoopDefaultMode);
}

// ------------------------------------------------------------
#define kDescendantProcess @"org_pqrs_Karabiner_DescendantProcess"

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  NSInteger isDescendantProcess = [[[NSProcessInfo processInfo] environment][kDescendantProcess] integerValue];
  setenv([kDescendantProcess UTF8String], "1", 1);

  // ------------------------------------------------------------
  if ([MigrationUtilities migrate:@[ @"org.pqrs.KeyRemap4MacBook" ]
           oldApplicationSupports:@[ @"KeyRemap4MacBook" ]
                         oldPaths:@[ @"/Applications/KeyRemap4MacBook.app" ]]) {
    [Relauncher relaunch];
  }

  // ------------------------------------------------------------
  if (![serverForUserspace_ register]) {
    // Relaunch when register is failed.
    NSLog(@"[ServerForUserspace register] is failed. Restarting process.");
    [NSThread sleepForTimeInterval:2];
    [Relauncher relaunch];
  }
  [Relauncher resetRelaunchedCount];

  // Wait until other apps connect to me.
  [NSThread sleepForTimeInterval:1];

  [[NSApplication sharedApplication] disableRelaunchOnLogin];

  if ([[NSUserDefaults standardUserDefaults] boolForKey:kShowIconInDock]) {
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
  }

  axnotifierManagerQueue_ = dispatch_queue_create("org.pqrs.Karabiner.axnotifierManagerQueue_", NULL);

  [preferencesManager_ load];

  [statusMessageManager_ setupStatusMessageManager];
  [statusbar_ refresh];
  [xmlCompiler_ reload];

  sessionObserver_ = [[SessionObserver alloc] init:1
      active:^{
        [statusMessageManager_ resetStatusMessage];
        [self registerIONotification];
        [self registerWakeNotification];
      }
      inactive:^{
        [statusMessageManager_ resetStatusMessage];
        [self unregisterIONotification];
        [self unregisterWakeNotification];
        [clientForKernelspace disconnect_from_kext];
      }];

  // ------------------------------------------------------------
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

  // ------------------------------------------------------------
  [self distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:nil];
  [self distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:nil];

  [updater_ checkForUpdatesInBackground:nil];

  // ------------------------------------------------------------
  [self manageAXNotifier:self];

  // ------------------------------------------------------------
  // Send kKarabinerServerDidLaunchNotification after launching AXNotifier.
  //
  // AXNotifier will be relaunched by kKarabinerServerDidLaunchNotification.
  // If we send the notification before launching AXNotifier,
  // two AXNotifier processes will be launched when AXNotifier is already running.
  //
  // * relaunched AXNotifier.
  // * AXNotifier launched by manageAXNotifier.

  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKarabinerServerDidLaunchNotification
                                                                 object:nil];

  // ------------------------------------------------------------
  // Open Preferences if Karabiner was launched by hand.
  // ------------------------------------------------------------
  {
    NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    if (![bundlePath isEqualToString:@"/Applications/Karabiner.app"]) {
      NSLog(@"Skip setStartAtLogin for %@", bundlePath);

      dispatch_async(dispatch_get_main_queue(), ^{
        NSAlert* alert = [NSAlert new];
        [alert setMessageText:@"Karabiner Alert"];
        [alert addButtonWithTitle:@"Close"];
        [alert setInformativeText:@"Karabiner.app should be located in /Applications/Karabiner.app.\nDo not move Karabiner.app into other folders."];
        [alert runModal];
      });

    } else {
      if (![StartAtLoginUtilities isStartAtLogin]) {
        [StartAtLoginUtilities setStartAtLogin:YES];

        if (!isDescendantProcess) {
          [preferencesController_ show];
        }
      }
    }
  }
}

- (BOOL)applicationShouldHandleReopen:(NSApplication*)theApplication hasVisibleWindows:(BOOL)flag {
  [preferencesController_ show];
  return YES;
}

- (void)dealloc {
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

// ------------------------------------------------------------
- (void)updateFocusedUIElementInformation:(NSDictionary*)information;
{
  @synchronized(self) {
    if (information) {
      // We ignore our investigation application.
      if ([information[@"BundleIdentifier"] isEqualToString:@"org.pqrs.Karabiner.EventViewer"]) return;

      focusedUIElementInformation_ = information;
    }

    workspaceAppIds_ = [xmlCompiler_ appids:focusedUIElementInformation_[@"BundleIdentifier"]];
    workspaceWindowNameIds_ = [xmlCompiler_ windownameids:focusedUIElementInformation_[@"WindowName"]];
    workspaceUIElementRoleId_ = @([workSpaceData_ getUIElementRole:focusedUIElementInformation_[@"UIElementRole"]]);

    [self send_workspacedata_to_kext];
  }
}

- (NSDictionary*)getFocusedUIElementInformation {
  @synchronized(self) {
    return focusedUIElementInformation_;
  }
}

- (NSArray*)getWorkspaceAppIds {
  @synchronized(self) {
    return workspaceAppIds_;
  }
}

- (NSArray*)getWorkspaceWindowNameIds {
  @synchronized(self) {
    return workspaceWindowNameIds_;
  }
}

- (NSArray*)getWorkspaceInputSourceIds {
  @synchronized(self) {
    return workspaceInputSourceIds_;
  }
}

- (NSDictionary*)getInputSourceInformation {
  @synchronized(self) {
    return inputSourceInformation_;
  }
}

// ------------------------------------------------------------
- (NSString*)AXNotifierPath {
  return @"/Applications/Karabiner.app/Contents/Applications/Karabiner_AXNotifier.app";
}

- (void)launchNewAXNotifier {
  dispatch_sync(axnotifierManagerQueue_, ^{
    NSString* path = [self AXNotifierPath];
    NSURL* url = [NSURL fileURLWithPath:path];
    // Set NSWorkspaceLaunchNewInstance because
    // AXNotifier might be running (terminating) immediately after terminateAXNotifier.
    NSWorkspaceLaunchOptions options = NSWorkspaceLaunchDefault | NSWorkspaceLaunchNewInstance;
    [[NSWorkspace sharedWorkspace] launchApplicationAtURL:url
                                                  options:options
                                            configuration:@{}
                                                    error:nil];
  });
}

- (void)terminateAXNotifiers {
  dispatch_sync(axnotifierManagerQueue_, ^{
    NSString* path = [self AXNotifierPath];
    NSString* bundleIdentifier = [[NSBundle bundleWithPath:path] bundleIdentifier];

    // If Karabiner has been moved into /Applications/Utilities, bundleIdentifier will be nil.

    if (bundleIdentifier) {
      NSArray* applications = [NSRunningApplication runningApplicationsWithBundleIdentifier:bundleIdentifier];
      for (NSRunningApplication* runningApplication in applications) {
        [runningApplication terminate];
      }
    }
  });
}

- (IBAction)restartAXNotifier:(id)sender {
  if (![[NSUserDefaults standardUserDefaults] boolForKey:kIsAXNotifierEnabled]) {
    NSAlert* alert = [NSAlert new];
    [alert setMessageText:@"Karabiner Alert"];
    [alert addButtonWithTitle:@"Close"];
    [alert setInformativeText:@"AXNotifier is disabled.\nPlease enable AXNotifier if you want to start."];
    [alert runModal];
    return;
  }

  [self terminateAXNotifiers];
  [self launchNewAXNotifier];
}

- (IBAction)manageAXNotifier:(id)sender {
  [self terminateAXNotifiers];

  if ([[NSUserDefaults standardUserDefaults] boolForKey:kIsAXNotifierEnabled]) {
    [self launchNewAXNotifier];
  }
}

- (IBAction)launchEventViewer:(id)sender {
  NSString* path = @"/Applications/Karabiner.app/Contents/Applications/EventViewer.app";
  [[NSWorkspace sharedWorkspace] launchApplication:path];
}

- (IBAction)launchMultiTouchExtension:(id)sender {
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Applications/Karabiner.app/Contents/Applications/Karabiner_multitouchextension.app"];
}

- (IBAction)launchUninstaller:(id)sender {
  NSString* path = @"/Library/Application Support/org.pqrs/Karabiner/uninstaller.applescript";
  [[[NSAppleScript alloc] initWithContentsOfURL:[NSURL fileURLWithPath:path] error:nil] executeAndReturnError:nil];
}

- (IBAction)openPreferences:(id)sender {
  [preferencesController_ show];
}

- (IBAction)openPrivateXML:(id)sender {
  // Open a directory which contains private.xml.
  NSString* path = [XMLCompiler get_private_xml_path];
  if ([path length] > 0) {
    [[NSWorkspace sharedWorkspace] openFile:[path stringByDeletingLastPathComponent]];
  }
}

- (IBAction)quit:(id)sender {
  NSAlert* alert = [NSAlert new];
  alert.messageText = @"Are you sure you want to quit Karabiner?";
  alert.informativeText = @"The changed key will be restored after Karabiner is quit.";
  [alert addButtonWithTitle:@"Quit"];
  [alert addButtonWithTitle:@"Cancel"];
  if ([alert runModal] != NSAlertFirstButtonReturn) {
    return;
  }

  [StartAtLoginUtilities setStartAtLogin:NO];
  [NSApp terminate:nil];
}

- (IBAction)relaunch:(id)sender {
  [Relauncher relaunch];
}

@end
