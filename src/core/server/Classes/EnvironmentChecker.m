#import "EnvironmentChecker.h"

@implementation EnvironmentChecker

static NSMutableDictionary* cache_;

+ (void) initialize
{
  cache_ = [NSMutableDictionary new];
}

+ (BOOL) checkDoubleCommand
{
  // If DoubleCommand was installed, return YES even if it removed.
  NSString* cachekey = @"DoubleCommand";
  if (cache_[cachekey]) {
    return YES;
  }

  {
    NSArray* paths = @[ @"/Library/StartupItems/DoubleCommand" ];
    for (NSString* path in paths) {
      if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
        cache_[cachekey] = @YES;
        return YES;
      }
    }
  }

  return NO;
}

+ (BOOL) checkKeyRemap4MacBook
{
  // If KeyRemap4MacBook was installed, return YES even if it removed.
  NSString* cachekey = @"KeyRemap4MacBook";
  if (cache_[cachekey]) {
    return YES;
  }

  {
    NSString* command = @"/usr/sbin/kextstat -l | /usr/bin/grep -q org.pqrs.driver.KeyRemap4MacBook";
    NSTask* task = [NSTask launchedTaskWithLaunchPath:@"/bin/sh" arguments:@[@"-c", command]];
    [task waitUntilExit];
    if ([task terminationStatus] == 0) {
      cache_[cachekey] = @YES;
      return YES;
    }
  }

  return NO;
}

+ (BOOL) checkSmoothMouse
{
  // If SmoothMouse was installed, return YES even if it removed.
  NSString* cachekey = @"SmoothMouse";
  if (cache_[cachekey]) {
    return YES;
  }

  {
    NSArray* paths = @[ @"/System/Library/Extensions/SmoothMouse.kext",
                        @"/Library/PreferencePanes/SmoothMouse.prefPane" ];
    for (NSString* path in paths) {
      if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
        cache_[cachekey] = @YES;
        return YES;
      }
    }
  }

  return NO;
}

@end
