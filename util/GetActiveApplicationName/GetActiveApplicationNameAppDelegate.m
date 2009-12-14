//
//  GetActiveApplicationNameAppDelegate.m
//  GetActiveApplicationName
//
//  Created by Takayama Fumihiko on 09/11/19.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "GetActiveApplicationNameAppDelegate.h"
#import "server_objc_part.h"

@implementation GetActiveApplicationNameAppDelegate

@synthesize window;

- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  NSRunningApplication* app = [[notification userInfo] objectForKey:@"NSWorkspaceApplicationKey"];
  if (! app) return;

  NSString* applicationName = [app bundleIdentifier];
  if (! applicationName) return;

  NSLog(@"switch to: %@", applicationName);
}

- (void) threadMain {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  for (;;) {
    char buffer[512];
    getActiveApplicationName(buffer, sizeof(buffer));
    NSLog(@"polling: %s", buffer);

    sleep(1);
  }

  [pool drain];
  [NSThread exit];
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  // ------------------------------------------------------------
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];

  [NSThread detachNewThreadSelector:@selector(threadMain)toTarget:self withObject:nil];
}

@end
