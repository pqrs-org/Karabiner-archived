// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookProtocol.h"

@interface org_pqrs_KeyRemap4MacBook_Client : NSObject {
  id proxy;
}

@property (assign) id<org_pqrs_KeyRemap4MacBook_Protocol> proxy;

@end
