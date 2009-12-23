//
//  KeyRemap4MacBookAppDelegate.m
//  KeyRemap4MacBook
//
//  Created by Takayama Fumihiko on 09/11/01.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "KeyRemap4MacBookAppDelegate.h"
#include "util.h"
#include "server_objc_part.h"
#include "server.hpp"
#include "Common.h"

@implementation KeyRemap4MacBookAppDelegate

@synthesize window;

- (void) threadMain {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  server_run();

  [pool drain];
  [NSThread exit];
}

- (void) configThreadMain {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  for (;;) {
    sysctl_load();
    sleep(1);
  }

  [pool drain];
  [NSThread exit];
}

// ------------------------------------------------------------
- (NSString*) execSysctl:(NSArray*)args
{
  NSString* path = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl";
  return [BUNDLEPREFIX(Common) getExecResult:path args:args];
}

- (int) isCheckUpdate
{
  NSString* result = [self execSysctl:[NSArray arrayWithObjects:@"checkupdate", nil]];
  return [result intValue];
}

// ------------------------------------------------------------
- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  NSRunningApplication* app = [[notification userInfo] objectForKey:@"NSWorkspaceApplicationKey"];
  if (! app) return;

  NSString* applicationName = [app bundleIdentifier];
  if (! applicationName) return;

  setCurrentApplicationType([applicationName UTF8String]);
}

// ------------------------------------------------------------
- (void) observer_NSWorkspaceSessionDidBecomeActiveNotification:(NSNotification*)notification
{
  NSLog(@"observer_NSWorkspaceSessionDidBecomeActiveNotification");

  // Note: The console user is "real login user" or "loginwindow",
  //       when NSWorkspaceSessionDidBecomeActiveNotification, NSWorkspaceSessionDidResignActiveNotification are called.
  sysctl_reset();
  sysctl_load();
}

- (void) observer_NSWorkspaceSessionDidResignActiveNotification:(NSNotification*)notification
{
  NSLog(@"observer_NSWorkspaceSessionDidResignActiveNotification");

  // Note: The console user is "real login user" or "loginwindow",
  //       when NSWorkspaceSessionDidBecomeActiveNotification, NSWorkspaceSessionDidResignActiveNotification are called.
  sysctl_reset();
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceSessionDidBecomeActiveNotification:)
                                                             name:NSWorkspaceSessionDidBecomeActiveNotification
                                                           object:nil];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceSessionDidResignActiveNotification:)
                                                             name:NSWorkspaceSessionDidResignActiveNotification
                                                           object:nil];

  // ------------------------------------------------------------
  char buffer[512];
  getActiveApplicationName(buffer, sizeof(buffer));
  setCurrentApplicationType(buffer);

  // ------------------------------------------------------------
  sysctl_reset();
  [NSThread detachNewThreadSelector:@selector(threadMain)
                           toTarget:self
                         withObject:nil];
  [NSThread detachNewThreadSelector:@selector(configThreadMain)
                           toTarget:self
                         withObject:nil];

  // ------------------------------------------------------------
  // Check for updates
  if ([self isCheckUpdate]) {
    NSLog(@"checkForUpdatesInBackground");
    [_suupdater checkForUpdatesInBackground];
  }
}

- (void) applicationWillTerminate:(NSNotification*)aNotification {
  NSLog(@"applicationWillTerminate");
  sysctl_reset();
}

@end
