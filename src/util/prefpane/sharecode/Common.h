/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "bundleprefix.h"
#import <Cocoa/Cocoa.h>

@interface BUNDLEPREFIX (Common) : NSObject

+ (NSString*) getExecResult:(NSString*)path args:(NSArray*)args;

@end
