// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "Common.h"
#import "SysctlWrapper.h"
#import "AdminAction.h"

@implementation Common

// ----------------------------------------------------------------------
+ (void) saveSetting
{
  [self initialize];

  char command[] = "/Library/org.pqrs/KeyRemap4MacBook/scripts/save.sh";
  [AdminAction execCommand:command];
}

+ (void) setSysctlInt:(NSString *)name value:(NSNumber *)value
{
  NSNumber *old = [SysctlWrapper getInt:name];
  if ([value isEqualToNumber:old]) return;

  char command[512];
  snprintf(command, sizeof(command), "/usr/sbin/sysctl -w '%s=%d'",
           [name cStringUsingEncoding:NSUTF8StringEncoding],
           [value intValue]);
  if (! [AdminAction execCommand:command]) return;

  [self saveSetting];
}

@end
