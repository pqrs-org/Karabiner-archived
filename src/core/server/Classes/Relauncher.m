#import "Relauncher.h"

@implementation Relauncher

#define kRelaunchedCount @"org_pqrs_KeyRemap4MacBook_RelaunchedCount"
#define kPreviousProcessVersion @"org_pqrs_KeyRemap4MacBook_PreviousProcessVersion"

// ------------------------------------------------------------
+ (void) setRelaunchedCount:(int)newvalue
{
  setenv([kRelaunchedCount UTF8String],
         [[NSString stringWithFormat:@"%d", newvalue] UTF8String],
         1);
}

+ (NSInteger) getRelaunchedCount
{
  return [[[NSProcessInfo processInfo] environment][kRelaunchedCount] integerValue];
}

+ (void) resetRelaunchedCount
{
  [self setRelaunchedCount:0];
}

// ------------------------------------------------------------
+ (NSString*) currentProcessVersion
{
  return [[NSBundle mainBundle] infoDictionary][@"CFBundleVersion"];
}

+ (void) setPreviousProcessVersion
{
  setenv([kPreviousProcessVersion UTF8String],
         [[self currentProcessVersion] UTF8String],
         1);
}

+ (BOOL) isEqualPreviousProcessVersionAndCurrentProcessVersion
{
  NSString* previous = [[NSProcessInfo processInfo] environment][kPreviousProcessVersion];
  if (! previous) return NO;

  NSString* current = [self currentProcessVersion];

  return [current isEqualToString:previous];
}

// ------------------------------------------------------------
+ (void) relaunch
{
  const int RETRY_COUNT = 5;

  NSInteger count = [self getRelaunchedCount];
  if (count < RETRY_COUNT) {
    NSLog(@"Relaunching (count:%d)", (int)(count));
    [self setRelaunchedCount:(int)(count + 1)];
    [self setPreviousProcessVersion];
    [NSTask launchedTaskWithLaunchPath:[[NSBundle mainBundle] executablePath] arguments:@[]];
  } else {
    NSLog(@"Give up relaunching.");
  }

  [NSApp terminate:self];
}

@end
