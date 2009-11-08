//
//  DumpInputModeToConsoleAppDelegate.m
//  DumpInputModeToConsole
//
//  Created by Takayama Fumihiko on 09/11/07.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "DumpInputModeToConsoleAppDelegate.h"
#import "server_objc_part.h"

@implementation DumpInputModeToConsoleAppDelegate

@synthesize window;

- (void) threadMain {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  for (;;) {
    char inputmodeName[128];
    getTISPropertyInputModeID(inputmodeName, sizeof(inputmodeName));
    if (*inputmodeName) {
      NSLog(@"inputmodeName = %s", inputmodeName);
    } else {
      NSLog(@"inputmodeName = <NULL>");
    }
    sleep(1);
  }

  [pool drain];
  [NSThread exit];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  [NSThread detachNewThreadSelector:@selector(threadMain) toTarget:self withObject:nil];
}

@end
