/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "Common.h"

@implementation BUNDLEPREFIX (Common)

+ (NSString*) getExecResult:(NSString*)path args:(NSArray*)args
{
  NSTask* task = [[[NSTask alloc] init] autorelease];
  NSPipe* pipe_ = [NSPipe pipe];
  [task setStandardOutput:pipe_];
  [task setLaunchPath:path];
  [task setArguments:args];
  [task launch];
  [task waitUntilExit];

  NSData* data = [[pipe_ fileHandleForReading] readDataToEndOfFile];
  NSString* result = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
  return result;
}

@end
