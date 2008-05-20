// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface BUNDLEPREFIX_Common : NSObject

+ (void) setSysctlInt:(NSString *)base name:(NSString *)name value:(NSNumber *)value sysctl_set:(NSString *)sysctl_set sysctl_ctl:(NSString *)sysctl_ctl;
+ (NSString *) getExecResult:(NSString *)path args:(NSArray *)args;

@end
