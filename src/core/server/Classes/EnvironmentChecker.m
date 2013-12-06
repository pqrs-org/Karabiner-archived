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
