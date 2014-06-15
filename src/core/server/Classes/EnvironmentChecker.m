#import "EnvironmentChecker.h"

@implementation EnvironmentChecker

static NSMutableDictionary* cache_;

+ (void) initialize
{
  cache_ = [NSMutableDictionary new];
}

+ (BOOL) checkDoubleCommand
{
  NSString* cachekey = @"DoubleCommand";
  if (cache_[cachekey] != nil) {
    return cache_[cachekey];
  }

  // ----------------------------------------
  cache_[cachekey] = @NO;

  NSArray* paths = @[ @"/Library/StartupItems/DoubleCommand" ];
  for (NSString* path in paths) {
    if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
      cache_[cachekey] = @YES;
      break;
    }
  }

  return cache_[cachekey];
}

+ (BOOL) checkKeyRemap4MacBook
{
  NSString* cachekey = @"KeyRemap4MacBook";
  if (cache_[cachekey] != nil) {
    return cache_[cachekey];
  }

  // ----------------------------------------
  cache_[cachekey] = @NO;

  NSString* command = @"/bin/test -n \"`/usr/sbin/kextstat -l -b org.pqrs.driver.KeyRemap4MacBook`\"";
  NSTask* task = [NSTask launchedTaskWithLaunchPath:@"/bin/sh" arguments:@[@"-c", command]];
  [task waitUntilExit];
  if ([task terminationStatus] == 0) {
    cache_[cachekey] = @YES;
  }

  return cache_[cachekey];
}

+ (BOOL) checkSmoothMouse
{
  NSString* cachekey = @"SmoothMouse";
  if (cache_[cachekey] != nil) {
    return cache_[cachekey];
  }

  // ----------------------------------------
  cache_[cachekey] = @NO;

  NSArray* paths = @[ @"/System/Library/Extensions/SmoothMouse.kext",
                       @"/Library/PreferencePanes/SmoothMouse.prefPane" ];
  for (NSString* path in paths) {
    if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
      cache_[cachekey] = @YES;
      break;
    }
  }

  return cache_[cachekey];
}

@end
