//
//  KeyRemap4MacBookAppDelegate.m
//  KeyRemap4MacBook
//
//  Created by Takayama Fumihiko on 09/12/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "KeyRemap4MacBookAppDelegate.h"

@implementation KeyRemap4MacBookAppDelegate

@synthesize window;

- (NSString*) getFeedURL
{
  NSInteger checkupdate = [[client_ proxy] checkForUpdatesMode];

  // ----------------------------------------
  // check nothing.
  if (checkupdate == 0) {
    NSLog(@"skip checkForUpdatesInBackground");
    return nil;
  }

  // ----------------------------------------
  // check beta & stable releases.

  // Once we check appcast.xml, SUFeedURL is stored in a user's preference file.
  // So that Sparkle gives priority to a preference over Info.plist,
  // we overwrite SUFeedURL here.
  if (checkupdate == 2) {
    return @"http://pqrs.org/macosx/keyremap4macbook/files/appcast-devel.xml";
  }

  return @"http://pqrs.org/macosx/keyremap4macbook/files/appcast.xml";
}

- (void) checkForUpdates:(BOOL)isBackground
{
  NSString* url = [self getFeedURL];
  if (! url) return;
  [suupdater_ setFeedURL:[NSURL URLWithString:url]];

  NSLog(@"checkForUpdates %@", url);
  if (isBackground) {
    [suupdater_ checkForUpdatesInBackground];
  } else {
    [suupdater_ checkForUpdates:nil];
  }
}

- (void) observer_check:(NSNotification*)aNotification
{
  [self checkForUpdates:NO];
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSString* observedObject = @"org.pqrs.KeyRemap4MacBook.updater";
  [[NSDistributedNotificationCenter defaultCenter] addObserver:self selector:@selector(observer_check:) name:@"check" object:observedObject];

  [self checkForUpdates:YES];
}

@end
