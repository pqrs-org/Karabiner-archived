//
//  KeyRemap4MacBook_serverAppDelegate.m
//  KeyRemap4MacBook_server
//
//  Created by Takayama Fumihiko on 09/11/01.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "KeyRemap4MacBook_serverAppDelegate.h"
#include "util.h"
#include "server_objc_part.h"
#include "server.hpp"

@implementation KeyRemap4MacBook_serverAppDelegate

@synthesize window;

- (void) threadMain {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  server_run();

  [pool drain];
  [NSThread exit];
}

- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  NSRunningApplication* app = [[notification userInfo] objectForKey:@"NSWorkspaceApplicationKey"];
  if (! app) return;

  NSString* applicationName = [app bundleIdentifier];
  if (! applicationName) return;

  setCurrentApplicationType([applicationName UTF8String]);
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                      selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                      name:NSWorkspaceDidActivateApplicationNotification
                                                      object:nil];

  char buffer[512];
  getActiveApplicationName(buffer, sizeof(buffer));
  setCurrentApplicationType(buffer);

  // ------------------------------------------------------------
  [NSThread detachNewThreadSelector:@selector(threadMain) toTarget:self withObject:nil];
}

@end
