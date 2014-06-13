#import "MigrationUtilities.h"
#import "StartAtLoginUtilities.h"

@implementation MigrationUtilities

+ (BOOL) migrateUserDefaults:(NSArray*)bundleIdentifiers
{
  NSString* currentBundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];

  // Migrate when old preferences exists.
  for (NSString* bundleIdentifier in bundleIdentifiers) {
    NSDictionary* d = [[NSUserDefaults standardUserDefaults] persistentDomainForName:bundleIdentifier];
    if ([d count] == 0) continue;

    NSLog(@"Migrate old preferences: (%@ -> %@)", bundleIdentifier, currentBundleIdentifier);

    [[NSUserDefaults standardUserDefaults] setPersistentDomain:d forName:currentBundleIdentifier];
    return YES;
  }

  return NO;
}

+ (void) migrateStartAtLogin:(NSArray*)appURLs
{
  BOOL startAtLogin = NO;
  for (NSURL* appURL in appURLs) {
    if ([StartAtLoginUtilities isStartAtLogin:appURL]) {
      startAtLogin = YES;

      NSLog(@"Remove %@ from Login Items.", appURL);
      [StartAtLoginUtilities setStartAtLogin:NO appURL:appURL];
    }
  }

  if (startAtLogin) {
    [StartAtLoginUtilities setStartAtLogin:YES];
  }
}

+ (void) migrate:(NSArray*)bundleIdentifiers appURLs:(NSArray*)appURLs
{
  // ----------------------------------------
  // Skip if already migrated.
  NSString* key = @"MigrationUtilitiesCurrentBundleIdentifer";
  NSString* currentBundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
  if ([[[NSUserDefaults standardUserDefaults] stringForKey:key] isEqualToString:currentBundleIdentifier]) {
    return;
  }

  // ----------------------------------------
  // Migrate
  if ([MigrationUtilities migrateUserDefaults:bundleIdentifiers]) {
    [MigrationUtilities migrateStartAtLogin:appURLs];
  }

  // ----------------------------------------
  // Set migrated
  [[NSUserDefaults standardUserDefaults] setObject:currentBundleIdentifier forKey:key];
}

@end
