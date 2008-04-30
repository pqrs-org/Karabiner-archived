// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface BUNDLEPREFIX_SysctlWrapper : NSObject

// Note:
// SysctlWrapper provides only the get methods.
// Because of the Authorization APIs, we use /usr/sbin/sysctl to set the value.

+ (NSString *) getString:(NSString *)name;
+ (NSNumber *) getInt:(NSString *)name;

@end
