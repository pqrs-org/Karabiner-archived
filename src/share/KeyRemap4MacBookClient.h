// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookProtocol.h"

@interface KeyRemap4MacBookClient : NSObject {
  id proxy_;
}

- (id<KeyRemap4MacBookProtocol>) proxy;

@end
