#import <Carbon/Carbon.h>
#import "AppDelegate.h"
#import "ClientForKernelspace.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
#import "ServerForUserspace.h"
#import "StatusBar.h"
#import "StatusWindow.h"
#import "Updater.h"
#import "WorkSpaceData.h"
#include <stdlib.h>

@implementation AppDelegate

@synthesize window;
@synthesize clientForKernelspace;

// ----------------------------------------
- (void) send_workspacedata_to_kext
{
  [clientForKernelspace send_workspacedata_to_kext:&bridgeworkspacedata_];
}

// ------------------------------------------------------------
- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  NSString* name = [WorkSpaceData getActiveApplicationName];
  if (name) {
    // We ignore our investigation application.
    if (! [name isEqualToString:@"org.pqrs.KeyRemap4MacBook.EventViewer"]) {
      bridgeworkspacedata_.applicationtype = [workSpaceData_ getApplicationType:name];
      [self send_workspacedata_to_kext];

      NSDictionary* userInfo = [NSDictionary dictionaryWithObject:name forKey:@"name"];

      [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookApplicationChangedNotification userInfo:userInfo];
    }
  }
}

- (void) distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:(NSNotification*)notification
{
  // [NSAutoreleasePool drain] is never called from NSDistributedNotificationCenter.
  // Therefore, we need to make own NSAutoreleasePool.
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    [WorkSpaceData refreshEnabledInputSources];
  }
  [pool drain];
}

- (void) distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:(NSNotification*)notification
{
  // [NSAutoreleasePool drain] is never called from NSDistributedNotificationCenter.
  // Therefore, we need to make own NSAutoreleasePool.
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    InputSource* inputSource = [WorkSpaceData getCurrentInputSource];
    [workSpaceData_ getInputSourceID:inputSource
                  output_inputSource:(&(bridgeworkspacedata_.inputsource))
            output_inputSourceDetail:(&(bridgeworkspacedata_.inputsourcedetail))];
    [self send_workspacedata_to_kext];

    NSMutableDictionary* userInfo = [[NSMutableDictionary new] autorelease];
    if ([inputSource languagecode]) {
      [userInfo setObject:[inputSource languagecode] forKey:@"languageCode"];
    }
    if ([inputSource inputSourceID]) {
      [userInfo setObject:[inputSource inputSourceID] forKey:@"inputSourceID"];
    }
    if ([inputSource inputModeID]) {
      [userInfo setObject:[inputSource inputModeID] forKey:@"inputModeID"];
    }
    [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookInputSourceChangedNotification userInfo:userInfo];
  }
  [pool drain];
}

// ------------------------------------------------------------
static void observer_IONotification(void* refcon, io_iterator_t iterator) {
  NSLog(@"observer_IONotification");

  AppDelegate* self = refcon;
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
}

- (void) unregisterIONotification {
  if (notifyport_) {
    if (loopsource_) {
      CFRunLoopSourceInvalidate(loopsource_);
      loopsource_ = nil;
    }
    IONotificationPortDestroy(notifyport_);
    notifyport_ = nil;
  }
}

- (void) registerIONotification {
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
                                                self,
                                                &it);
  if (kernResult != kIOReturnSuccess) {
    NSLog(@"[ERROR] IOServiceAddMatchingNotification failed");
    return;
  }
  observer_IONotification(self, it);

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
  NSLog(@"observer_NSWorkspaceSessionDidBecomeActiveNotification");

  [statusWindow_ resetStatusMessage];

  [self registerIONotification];
}

- (void) observer_NSWorkspaceSessionDidResignActiveNotification:(NSNotification*)notification
{
  NSLog(@"observer_NSWorkspaceSessionDidResignActiveNotification");

  [statusWindow_ resetStatusMessage];

  [self unregisterIONotification];
  [clientForKernelspace disconnect_from_kext];
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  if (! [serverForUserspace_ register]) {
    // Quit when register is failed.
    // We wait 2 second before quit to avoid consecutive restarting from launchd.
    NSLog(@"[ServerForUserspace register] is failed. Restarting process.");
    [NSThread sleepForTimeInterval:2];
    [NSApp terminate:nil];
  }
  // Wait until other apps connect to me.
  [NSThread sleepForTimeInterval:1];

  [preferencesManager_ load];

  [self registerIONotification];

  [statusWindow_ resetStatusMessage];
  [statusbar_ refresh];
  [xmlCompiler_ reload];

  // ------------------------------------------------------------
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                selector:@selector(distributedObserver_kTISNotifyEnabledKeyboardInputSourcesChanged:)
                                                                    name:(NSString*)(kTISNotifyEnabledKeyboardInputSourcesChanged)
                                                                  object:nil];

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                selector:@selector(distributedObserver_kTISNotifySelectedKeyboardInputSourceChanged:)
                                                                    name:(NSString*)(kTISNotifySelectedKeyboardInputSourceChanged)
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
}

- (void) dealloc
{
  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter removeObserver:self];
  [[NSNotificationCenter defaultCenter] removeObserver:self];

  [super dealloc];
}

@end
