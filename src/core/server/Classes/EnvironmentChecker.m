#import "EnvironmentChecker.h"

@implementation EnvironmentChecker

+ (BOOL) checkDoubleCommand
{
  NSString* path = @"/Library/StartupItems/DoubleCommand";
  return [[NSFileManager defaultManager] fileExistsAtPath:path];
}

+ (BOOL) checkSmoothMouse
{
  NSString* path = @"/System/Library/Extensions/SmoothMouse.kext";
  return [[NSFileManager defaultManager] fileExistsAtPath:path];
}

@end
