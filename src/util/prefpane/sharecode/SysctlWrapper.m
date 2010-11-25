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

@end
