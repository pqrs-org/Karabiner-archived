// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "KarabinerProtocol.h"

@interface KarabinerClient : NSObject {
  id proxy_;
}

- (id<KarabinerProtocol>)proxy;

@end
