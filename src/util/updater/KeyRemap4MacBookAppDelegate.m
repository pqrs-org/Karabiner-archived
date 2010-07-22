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
  return [BUNDLEPREFIX (Common) getExecResult:path args:args];
}

- (int) getCheckUpdate
{
  NSString* result = [self execSysctl:[NSArray arrayWithObjects:@"checkupdate", nil]];
  return [result intValue];
}

- (void) checkUpdate:(NSNotification*)notification {
  int checkupdate = [self getCheckUpdate];

  // ----------------------------------------
  // check nothing.
  if (checkupdate == 0) {
    NSLog(@"skip checkForUpdatesInBackground");
    return;
  }

  // ----------------------------------------
  // check beta & stable releases.

  // Once we check appcast.xml, SUFeedURL is stored in a user's preference file.
  // So that Sparkle gives priority to a preference over Info.plist,
  // we overwrite SUFeedURL here.
  NSString* feedurl = @"http://pqrs.org/macosx/keyremap4macbook/files/appcast.xml";
  if (checkupdate == 2) {
    feedurl = @"http://pqrs.org/macosx/keyremap4macbook/files/appcast-devel.xml";
  }
  [_suupdater setFeedURL:[[[NSURL alloc] initWithString:feedurl] autorelease]];

  NSLog(@"checkForUpdatesInBackground %@", [[_suupdater feedURL] absoluteString]);
  [_suupdater checkForUpdatesInBackground];
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSString* observedObject = @"org.pqrs.KeyRemap4MacBook.updater";
  [[NSDistributedNotificationCenter defaultCenter] addObserver:self selector:@selector(checkUpdate:) name:@"check" object:observedObject];

  [self checkUpdate:aNotification];
}

@end
