// -*- Mode: objc -*-

@import Cocoa;

@interface SharedCheckboxTree : NSObject <NSCoding, NSCopying>

@property NSNumber* id;
@property(copy) NSArray* children;

- (instancetype)initWithId:(NSNumber*)id
                  children:(NSArray*)children;

@end
