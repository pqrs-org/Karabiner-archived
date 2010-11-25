/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "bundleprefix.h"
#import <Cocoa/Cocoa.h>

@interface BUNDLEPREFIX (SysctlWrapper) : NSObject

+ (NSString*) getString:(NSString*)name;
+ (NSNumber*) getInt:(NSString*)name;

@end
