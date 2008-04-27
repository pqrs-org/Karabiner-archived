// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "ConfigList.h"

@implementation ConfigList

int cache_count = -1;
NSMutableDictionary *cache_name = nil;

+ (NSString *) getExecResult:(NSArray *)args
{
  NSTask *task = [[NSTask alloc] init];
  NSPipe *pipe = [NSPipe pipe];
  [task setStandardOutput:pipe];
  [task setLaunchPath:@"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl"];
  [task setArguments:args];
  [task launch];
  [task waitUntilExit];

  NSData *data = [[pipe fileHandleForReading] readDataToEndOfFile];
  NSString *result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
  return result;
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
    NSString *result = [ConfigList getExecResult:[NSArray arrayWithObjects:@"count", nil]];
    cache_count = [result intValue];
  }
  return cache_count;
}

+ (int) isStatusbarEnable
{
  NSString *result = [ConfigList getExecResult:[NSArray arrayWithObjects:@"statusbar", nil]];
  return [result intValue];
}

+ (int) getSelectedIndex
{
  NSString *result = [ConfigList getExecResult:[NSArray arrayWithObjects:@"current", nil]];
  return [result intValue];
}

+ (NSString *) getName:(int)index
{
  if (cache_name == nil) {
    cache_name = [[NSMutableDictionary alloc] init];
  }

  NSString *idx = [[[NSString alloc] initWithFormat:@"%d", index] autorelease];
  NSString *result = [cache_name objectForKey:idx];
  if (result == nil) {
    result = [ConfigList getExecResult:[NSArray arrayWithObjects:@"getname", idx, nil]];
    [cache_name setObject:result forKey:idx];
  }
  return result;
}

+ (void) setName:(int)index newName:(NSString *)name
{
  if ([name isEqualToString:@""]) return;
  NSString *idx = [[[NSString alloc] initWithFormat:@"%d", index] autorelease];
  [ConfigList getExecResult:[NSArray arrayWithObjects:@"rename", idx, name, nil]];

  [ConfigList refresh];
}

+ (void) add
{
  [ConfigList getExecResult:[NSArray arrayWithObjects:@"add", nil]];
  [ConfigList refresh];
}

+ (void) delete:(int)index
{
  NSString *selectedIndex = [[[NSString alloc] initWithFormat:@"%d", index] autorelease];
  [ConfigList getExecResult:[NSArray arrayWithObjects:@"delete", selectedIndex, nil]];

  [ConfigList refresh];
}

+ (void) select:(NSString *)idx
{
  [ConfigList getExecResult:[NSArray arrayWithObjects:@"select", idx, nil]];
  [ConfigList getExecResult:[NSArray arrayWithObjects:@"load", nil]];
}

@end
