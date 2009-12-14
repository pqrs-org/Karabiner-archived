/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#include <stdio.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#import "SysctlWrapper.h"

@implementation BUNDLEPREFIX (SysctlWrapper)

+ (NSString*) getString:(NSString*)name
{
  const char* cstr = [name UTF8String];

  char buf[512];
  size_t len = sizeof(buf);
  int error = sysctlbyname(cstr, &buf, &len, NULL, 0);

  if (error) {
    NSLog(@"SysctlWrapper::getString error = %d(%d)", error, errno);
    return nil;
  }
  if (! buf[0]) return nil;

  return [[[NSString alloc] initWithUTF8String:buf] autorelease];
}

+ (NSNumber*) getInt:(NSString*)name
{
  const char* cstr = [name UTF8String];

  int value;
  size_t len = sizeof(value);
  int error = sysctlbyname(cstr, &value, &len, NULL, 0);
  if (error) return nil;

  return [[[NSNumber alloc] initWithInt:value] autorelease];
}

+ (void) setSysctlInt:(NSString*)base name:(NSString*)name value:(NSNumber*)value sysctl_set:(NSString*)sysctl_set sysctl_ctl:(NSString*)sysctl_ctl
{
  NSString* entry = [NSString stringWithFormat:@"%@.%@", base, name];

  NSNumber* old = [BUNDLEPREFIX(SysctlWrapper) getInt:entry];
  if (old == nil) return;
  if ([value isEqualToNumber:old]) return;

  NSArray* args_set = [NSArray arrayWithObjects:name, [value stringValue], nil];
  NSTask* task_set = [NSTask launchedTaskWithLaunchPath:sysctl_set arguments:args_set];
  [task_set waitUntilExit];

  NSArray* args_save = [NSArray arrayWithObjects:@"save", nil];
  NSTask* task_save = [NSTask launchedTaskWithLaunchPath:sysctl_ctl arguments:args_save];
  [task_save waitUntilExit];
}

@end
