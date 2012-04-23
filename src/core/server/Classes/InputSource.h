// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>

@interface InputSource : NSObject {
  NSString* bcp47;
  NSString* inputSourceID;
  NSString* inputModeID;

  TISInputSourceRef inputSource_;
}

@property (assign, readwrite) NSString* bcp47;
@property (assign, readwrite) NSString* inputSourceID;
@property (assign, readwrite) NSString* inputModeID;

- (id) initWithTISInputSourceRef:(TISInputSourceRef)ref;
- (void) select;

@end
