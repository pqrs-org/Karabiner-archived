// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;
#import "KarabinerProtocol.h"

@interface KarabinerClient : NSObject

- (NSDistantObject<KarabinerProtocol>*)proxy;

@end
