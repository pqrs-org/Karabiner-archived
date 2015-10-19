#import "PreferencesManager.h"
#import "Sparkle/SUUpdater.h"
#import "Updater.h"

@interface Updater () {
  SUUpdater* suupdater_;
}
@end

@implementation Updater

- (id)init {
  self = [super init];

  if (self) {
    suupdater_ = [SUUpdater new];
  }

  return self;
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

- (void)check:(BOOL)isBackground {
  if (![preferencesManager_ isCheckForUpdates]) {
    NSLog(@"skip checkForUpdates");
    return;
  }

  NSString* url = [self getFeedURL:NO];
  [suupdater_ setFeedURL:[NSURL URLWithString:url]];

  NSLog(@"checkForUpdates %@", url);
  if (isBackground) {
    [suupdater_ checkForUpdatesInBackground];
  } else {
    [suupdater_ checkForUpdates:nil];
  }
}

- (IBAction)checkForUpdates:(id)sender {
  [self check:NO];
}

- (IBAction)checkForUpdatesInBackground:(id)sender {
  [self check:YES];
}

- (IBAction)checkForUpdatesStableOnly:(id)sender {
  NSString* url = [self getFeedURL:NO];
  [suupdater_ setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [suupdater_ checkForUpdates:nil];
}

- (IBAction)checkForUpdatesWithBetaVersion:(id)sender {
  NSString* url = [self getFeedURL:YES];
  [suupdater_ setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [suupdater_ checkForUpdates:nil];
}

@end
