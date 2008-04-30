// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "Common.h"
#import "SysctlWrapper.h"
#import "defs.h"

@implementation org_pqrs_Common

+ (void) setSysctlInt:(NSString *)base name:(NSString *)name value:(NSNumber *)value sysctl_set:(NSString *)sysctl_set sysctl_ctl:(NSString *)sysctl_ctl
{
  NSString *entry = [NSString stringWithFormat:@"%@.%@", base, name];

  NSNumber *old = [SysctlWrapper getInt:entry];
  if (old == nil) return;
  if ([value isEqualToNumber:old]) return;

  NSArray *args_set = [NSArray arrayWithObjects:name, [value stringValue], nil];
  NSTask *task_set = [NSTask launchedTaskWithLaunchPath:sysctl_set arguments:args_set];
  [task_set waitUntilExit];

  NSArray *args_save = [NSArray arrayWithObjects:@"save", nil];
  NSTask *task_save = [NSTask launchedTaskWithLaunchPath:sysctl_ctl arguments:args_save];
  [task_save waitUntilExit];
}

@end
