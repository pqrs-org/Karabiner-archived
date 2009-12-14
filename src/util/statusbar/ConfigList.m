// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "ConfigList.h"
#import "Common.h"

@implementation ConfigList

int cache_count = -1;
NSMutableDictionary* cache_name = nil;


+ (NSString*) execSysctl:(NSArray*)args
{
  NSString* path = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl";
  return [BUNDLEPREFIX(Common) getExecResult:path args:args];
}

// ----------------------------------------------------------------------
+ (void) refresh
{
  cache_count = -1;
  if (cache_name != nil) {
    [cache_name removeAllObjects];
  }
}

+ (int) getSize
{
  if (cache_count == -1) {
    NSString* result = [ConfigList execSysctl:[NSArray arrayWithObjects:@"count", nil]];
    cache_count = [result intValue];
  }
  return cache_count;
}

+ (int) isStatusbarEnable
{
  NSString* result = [ConfigList execSysctl:[NSArray arrayWithObjects:@"statusbar", nil]];
  return [result intValue];
}

+ (int) getSelectedIndex
{
  NSString* result = [ConfigList execSysctl:[NSArray arrayWithObjects:@"current", nil]];
  return [result intValue];
}

+ (NSString*) getName:(int)index_
{
  if (cache_name == nil) {
    cache_name = [[NSMutableDictionary alloc] init];
  }

  NSString* idx = [[[NSString alloc] initWithFormat:@"%d", index_] autorelease];
  NSString* result = [cache_name objectForKey:idx];
  if (result == nil) {
    result = [ConfigList execSysctl:[NSArray arrayWithObjects:@"getname", idx, nil]];
    [cache_name setObject:result forKey:idx];
  }
  return result;
}

+ (void) setName:(int)index_ newName:(NSString*)name
{
  if ([name isEqualToString:@""]) return;
  NSString* idx = [[[NSString alloc] initWithFormat:@"%d", index_] autorelease];
  [ConfigList execSysctl:[NSArray arrayWithObjects:@"rename", idx, name, nil]];

  [ConfigList refresh];
}

+ (void) add
{
  [ConfigList execSysctl:[NSArray arrayWithObjects:@"add", nil]];
  [ConfigList refresh];
}

+ (void) delete:(NSInteger)index_
{
  NSString* selectedIndex = [[[NSString alloc] initWithFormat:@"%d", index_] autorelease];
  [ConfigList execSysctl:[NSArray arrayWithObjects:@"delete", selectedIndex, nil]];

  [ConfigList refresh];
}

+ (void) select:(NSString*)idx
{
  [ConfigList execSysctl:[NSArray arrayWithObjects:@"select", idx, nil]];
  [ConfigList execSysctl:[NSArray arrayWithObjects:@"load", nil]];
}

@end
