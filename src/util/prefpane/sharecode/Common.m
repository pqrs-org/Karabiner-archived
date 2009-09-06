/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "Common.h"

@implementation BUNDLEPREFIX(Common)

+ (NSString *) getExecResult:(NSString *)path args:(NSArray *)args
{
  NSTask *task = [[NSTask alloc] init];
  NSPipe *pipe_ = [NSPipe pipe];
  [task setStandardOutput:pipe_];
  [task setLaunchPath:path];
  [task setArguments:args];
  [task launch];
  [task waitUntilExit];

  NSData *data = [[pipe_ fileHandleForReading] readDataToEndOfFile];
  NSString *result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
  return result;
}

@end
