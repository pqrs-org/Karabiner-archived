// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface Common : NSObject

+ (void) setSysctlInt:(NSString *)name value:(NSNumber *)value;

@end
