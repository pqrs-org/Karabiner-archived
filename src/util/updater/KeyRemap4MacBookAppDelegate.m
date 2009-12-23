//
//  KeyRemap4MacBookAppDelegate.m
//  KeyRemap4MacBook
//
//  Created by Takayama Fumihiko on 09/12/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "KeyRemap4MacBookAppDelegate.h"
#import "Common.h"

@implementation KeyRemap4MacBookAppDelegate

@synthesize window;

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

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  if ([self isCheckUpdate]) {
    NSLog(@"checkForUpdatesInBackground");
    [_suupdater checkForUpdatesInBackground];
  }
  //[NSApp terminate:self];
}

@end
