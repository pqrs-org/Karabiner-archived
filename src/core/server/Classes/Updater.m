@import Sparkle;
#import "Updater.h"
#import "PreferencesModel.h"

@interface Updater ()

@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property SUUpdater* suupdater;

@end

@implementation Updater

- (instancetype)init {
  self = [super init];

  if (self) {
    self.suupdater = [SUUpdater new];
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
  if (!self.preferencesModel.checkForUpdates) {
    NSLog(@"skip checkForUpdates");
    return;
  }

  NSString* url = [self getFeedURL:NO];
  [self.suupdater setFeedURL:[NSURL URLWithString:url]];

  NSLog(@"checkForUpdates %@", url);
  if (isBackground) {
    [self.suupdater checkForUpdatesInBackground];
  } else {
    [self.suupdater checkForUpdates:nil];
  }
}

- (void)checkForUpdatesInBackground {
  [self check:YES];
}

- (void)checkForUpdatesStableOnly {
  NSString* url = [self getFeedURL:NO];
  [self.suupdater setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [self.suupdater checkForUpdates:nil];
}

- (void)checkForUpdatesWithBetaVersion {
  NSString* url = [self getFeedURL:YES];
  [self.suupdater setFeedURL:[NSURL URLWithString:url]];
  NSLog(@"checkForUpdates %@", url);
  [self.suupdater checkForUpdates:nil];
}

@end
