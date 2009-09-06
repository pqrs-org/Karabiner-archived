// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "bundleprefix.h"
#import <Cocoa/Cocoa.h>
#import <Security/Authorization.h>
#import <Security/AuthorizationTags.h>

@interface BUNDLEPREFIX(AdminAction) : NSObject

+ (BOOL) execCommand:(char *)command;

@end
