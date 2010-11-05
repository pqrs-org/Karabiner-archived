// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface KeyCode : NSObject {
  NSMutableDictionary* dict_;
}

- (unsigned int) unsignedIntValue:(NSString*)name;

@end
