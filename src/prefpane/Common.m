// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "Common.h"
#import "SysctlWrapper.h"

@implementation Common

+ (void) setSysctlInt:(NSString *)name value:(NSNumber *)value
{
  NSString *entry = [NSString stringWithFormat:@"keyremap4macbook.%@", name];

  NSNumber *old = [SysctlWrapper getInt:entry];
  if (old == nil) return;
  if ([value isEqualToNumber:old]) return;

  NSArray *args_set = [NSArray arrayWithObjects:name, [value stringValue], nil];
  NSTask *task_set = [NSTask launchedTaskWithLaunchPath:@"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set" arguments:args_set];
  [task_set waitUntilExit];

  NSArray *args_save = [NSArray arrayWithObjects:nil];
  NSTask *task_save = [NSTask launchedTaskWithLaunchPath:@"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_save" arguments:args_save];
  [task_save waitUntilExit];
}

@end
