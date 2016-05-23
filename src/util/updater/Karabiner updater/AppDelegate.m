#import "AppDelegate.h"
@import Sparkle;

@interface AppDelegate ()

@property(weak) IBOutlet SUUpdater* suUpdater;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  NSArray* arguments = [[NSProcessInfo processInfo] arguments];

  if ([arguments count] == 2) {
    NSString* mode = arguments[1];

    if ([mode isEqualToString:@"checkForUpdatesInBackground"]) {
      [self checkForUpdatesInBackground];
      return;
    } else if ([mode isEqualToString:@"checkForUpdatesStableOnly"]) {
      [self checkForUpdatesStableOnly];
      return;
    } else if ([mode isEqualToString:@"checkForUpdatesWithBetaVersion"]) {
      [self checkForUpdatesWithBetaVersion];
      return;
    }
  }

  NSLog(@"Usage: Karabiner.app checkForUpdatesInBackground|checkForUpdatesStableOnly|checkForUpdatesWithBetaVersion");
}

- (NSString*)getFeedURL:(BOOL)includingBetaVersions {
  // ----------------------------------------
  // check beta & stable releases.

  // Once we check appcast.xml, SUFeedURL is stored in a user's preference file.
  // So that Sparkle gives priority to a preference over Info.plist,
  // we overwrite SUFeedURL here.
  if (includingBetaVersions) {
    return @"https://pqrs.org/osx/karabiner/files/appcast-devel.xml";
  }

  return @"https://pqrs.org/osx/karabiner/files/appcast.xml";
}

- (void)checkForUpdatesInBackground {
  NSString* url = [self getFeedURL:NO];
  [self.suUpdater setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [self.suUpdater checkForUpdatesInBackground];
}

- (void)checkForUpdatesStableOnly {
  NSString* url = [self getFeedURL:NO];
  [self.suUpdater setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [self.suUpdater checkForUpdates:nil];
}

- (void)checkForUpdatesWithBetaVersion {
  NSString* url = [self getFeedURL:YES];
  [self.suUpdater setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [self.suUpdater checkForUpdates:nil];
}

@end
