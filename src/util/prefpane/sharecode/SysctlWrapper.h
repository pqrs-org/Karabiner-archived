/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "bundleprefix.h"
#import <Cocoa/Cocoa.h>

@interface BUNDLEPREFIX(SysctlWrapper) : NSObject

+ (NSString *) getString:(NSString *)name;
+ (NSNumber *) getInt:(NSString *)name;

+ (void) setSysctlInt:(NSString *)base name:(NSString *)name value:(NSNumber *)value sysctl_set:(NSString *)sysctl_set sysctl_ctl:(NSString *)sysctl_ctl;

@end
