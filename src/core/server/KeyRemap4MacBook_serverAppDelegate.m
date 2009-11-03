//
//  KeyRemap4MacBook_serverAppDelegate.m
//  KeyRemap4MacBook_server
//
//  Created by Takayama Fumihiko on 09/11/01.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "KeyRemap4MacBook_serverAppDelegate.h"
#include "util.h"

@implementation KeyRemap4MacBook_serverAppDelegate

@synthesize window;

- (void) threadMain {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  runServer();

  [pool drain];
  [NSThread exit];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  if (! verifyUser()) {
    NSLog(@"[ERROR] invalid user");
    [NSApp terminate:self];
  }

  [NSThread detachNewThreadSelector:@selector(threadMain) toTarget:self withObject:nil];
}

@end
