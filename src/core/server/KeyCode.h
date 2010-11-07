// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface KeyCode : NSObject {
  NSMutableDictionary* dict_;
}

- (unsigned int) unsignedIntValue:(NSString*)name;
- (NSNumber*) numberValue:(NSString*)name;

- (void) append:(NSString*)name newvalue:(unsigned int)newvalue;
- (void) append:(NSString*)type name:(NSString*)name;

+ (NSString*) normalizeName:(NSString*)name;

@end
