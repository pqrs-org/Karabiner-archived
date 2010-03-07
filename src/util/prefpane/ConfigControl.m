// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "ConfigControl.h"
#import "Common.h"

@implementation ConfigControl

+ (NSString*) execSysctl:(NSArray*)args
{
  NSString* path = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl";
  return [BUNDLEPREFIX(Common) getExecResult:path args:args];
}

// ----------------------------------------------------------------------
+ (int) isStatusbarEnable
{
  NSString* result = [ConfigControl execSysctl:[NSArray arrayWithObjects:@"statusbar", nil]];
  return [result intValue];
}

// ----------------------------------------------------------------------
+ (NSArray*) getConfigList
{
  NSString* result = [ConfigControl execSysctl:[NSArray arrayWithObjects:@"list", nil]];
  return [result componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
}

+ (void) setName:(int)index_ newName:(NSString*)name
{
  name = [name stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
  if ([name isEqualToString:@""]) {
    name = @"NewItem";
  }
  NSString* idx = [[[NSString alloc] initWithFormat:@"%d", index_] autorelease];
  [ConfigControl execSysctl:[NSArray arrayWithObjects:@"rename", idx, name, nil]];
}

+ (void) add
{
  [ConfigControl execSysctl:[NSArray arrayWithObjects:@"add", nil]];
}

+ (void) delete:(NSInteger)index_
{
  NSString* selectedIndex = [[[NSString alloc] initWithFormat:@"%d", index_] autorelease];
  [ConfigControl execSysctl:[NSArray arrayWithObjects:@"delete", selectedIndex, nil]];
}

+ (void) select:(NSString*)idx
{
  [ConfigControl execSysctl:[NSArray arrayWithObjects:@"select", idx, nil]];
  [ConfigControl execSysctl:[NSArray arrayWithObjects:@"load", nil]];
}

@end
