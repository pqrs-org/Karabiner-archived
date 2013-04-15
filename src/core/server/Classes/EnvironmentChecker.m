#import "EnvironmentChecker.h"

@implementation EnvironmentChecker

+ (BOOL) checkDoubleCommand
{
  {
    NSString* path = @"/Library/StartupItems/DoubleCommand";
    if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
      return YES;
    }
  }
  return NO;
}

+ (BOOL) checkSmoothMouse
{
  {
    NSString* path = @"/System/Library/Extensions/SmoothMouse.kext";
    if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
      return YES;
    }
  }
  {
    NSString* path = @"/Library/PreferencePanes/SmoothMouse.prefPane";
    if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
      return YES;
    }
  }
  return NO;
}

@end
