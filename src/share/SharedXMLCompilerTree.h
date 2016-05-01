// -*- Mode: objc -*-

@import Cocoa;

@interface SharedXMLCompilerTree : NSObject <NSCoding, NSCopying>

@property NSNumber* id;
@property(copy) NSArray* children;

- (instancetype)initWithId:(NSNumber*)id
                  children:(NSArray*)children;

@end
