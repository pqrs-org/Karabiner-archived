// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import <Security/Authorization.h>
#import <Security/AuthorizationTags.h>

@interface AdminAction : NSObject
{
  AuthorizationRef _authorizationRef;
}

- (BOOL) execCommand:(char *)command;

@end
