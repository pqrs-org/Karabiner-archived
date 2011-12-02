// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface NSString (HashBraces)

- (NSString*) stringByReplacingHashBracesOccurrencesOfDictionary:(NSDictionary*)replacementDictionary;

@end
