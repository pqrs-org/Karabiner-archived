// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "SysctlWrapper.h"

@implementation org_pqrs_SysctlWrapper

+ (NSString *) getString:(NSString *)name
{
  const char *cstr = [name UTF8String];

  char buf[512];
  int len = sizeof(buf);
  int error = sysctlbyname(cstr, &buf, &len, NULL, 0);
  if (error) return nil;
  if (! buf[0]) return nil;

  return [[[NSString alloc] initWithUTF8String:buf] autorelease];
}

+ (NSNumber *) getInt:(NSString *)name
{
  const char *cstr = [name UTF8String];

  int value;
  int len = sizeof(value);
  int error = sysctlbyname(cstr, &value, &len, NULL, 0);
  if (error) return nil;

  return [[[NSNumber alloc] initWithInt:value] autorelease];
}

@end
