#import "MigrationUtilities.h"
#import "StartAtLoginUtilities.h"

@implementation MigrationUtilities

// This value must be same as Sparkle's value.
+ (NSString*)applicationSupportName {
  NSBundle* bundle = [NSBundle mainBundle];

  NSString* name = [bundle objectForInfoDictionaryKey:@"CFBundleDisplayName"];
  if ([name length] > 0) return name;

  name = [bundle objectForInfoDictionaryKey:@"CFBundleName"];
  if ([name length] > 0) return name;

  return [[[NSFileManager defaultManager] displayNameAtPath:[bundle bundlePath]] stringByDeletingPathExtension];
}

+ (BOOL)migrateUserDefaults:(NSArray*)oldBundleIdentifiers {
  NSString* currentBundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];

  // Migrate when old preferences exists.
  for (NSString* bundleIdentifier in oldBundleIdentifiers) {
    NSDictionary* d = [[NSUserDefaults standardUserDefaults] persistentDomainForName:bundleIdentifier];
    if ([d count] == 0) continue;

    NSLog(@"Migrate old preferences: (%@ -> %@)", bundleIdentifier, currentBundleIdentifier);

    [[NSUserDefaults standardUserDefaults] setPersistentDomain:d forName:currentBundleIdentifier];
    return YES;
  }

  return NO;
}

+ (void)migrateApplicationSupport:(NSArray*)oldApplicationSupports {
  NSFileManager* filemanager = [NSFileManager defaultManager];
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
  if ([paths count] == 0) return;

  NSString* path = paths[0];

  for (NSString* old in oldApplicationSupports) {
    NSString* oldpath = [path stringByAppendingPathComponent:old];
    if ([filemanager fileExistsAtPath:oldpath]) {
      NSString* name = [MigrationUtilities applicationSupportName];
      if ([name length] == 0) return;

      NSLog(@"Migrate old Application Support: (%@ -> %@)", old, name);

      NSString* newpath = [path stringByAppendingPathComponent:name];
      NSError* error = nil;

      if ([filemanager fileExistsAtPath:newpath]) {
        if (![filemanager removeItemAtPath:newpath error:&error]) {
          NSLog(@"Removing %@ is failed: %@", newpath, error);
          return;
        }
      }
      if (![filemanager copyItemAtPath:oldpath toPath:newpath error:&error]) {
        NSLog(@"Copying %@ to %@ is failed: %@", oldpath, newpath, error);
        return;
      }

      return;
    }
  }
}

+ (void)migrateStartAtLogin:(NSArray*)oldPaths {
  // Note: This method does not work properly in major case.
  // Because isStartAtLogin will be failed with deleted file URL.
  // (== LSSharedFileListItemResolve will be failed with deleted file URL.)
  //
  // So, this method works only when both old app and new app exist.

  BOOL startAtLogin = NO;
  for (NSString* path in oldPaths) {
    NSURL* url = [NSURL fileURLWithPath:path];
    if ([StartAtLoginUtilities isStartAtLogin:url]) {
      startAtLogin = YES;

      NSLog(@"Remove %@ from Login Items.", url);
      [StartAtLoginUtilities setStartAtLogin:NO appURL:url];
    }
  }

  if (startAtLogin) {
    [StartAtLoginUtilities setStartAtLogin:YES];
  }
}

+ (BOOL)migrate:(NSArray*)oldBundleIdentifiers
    oldApplicationSupports:(NSArray*)oldApplicationSupports
                  oldPaths:(NSArray*)oldPaths;
{
  // ----------------------------------------
  // Skip if already migrated.
  NSString* key = @"MigrationUtilitiesCurrentBundleIdentifer";
  NSString* currentBundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
  if ([[[NSUserDefaults standardUserDefaults] stringForKey:key] isEqualToString:currentBundleIdentifier]) {
    return NO;
  }

  // ----------------------------------------
  // Migrate
  BOOL migrated = NO;

  if ([MigrationUtilities migrateUserDefaults:oldBundleIdentifiers]) {
    migrated = YES;

    [MigrationUtilities migrateApplicationSupport:oldApplicationSupports];
    [MigrationUtilities migrateStartAtLogin:oldPaths];
  }

  // ----------------------------------------
  // Set migrated
  [[NSUserDefaults standardUserDefaults] setObject:currentBundleIdentifier forKey:key];

  return migrated;
}

@end
