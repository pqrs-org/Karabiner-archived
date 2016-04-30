// -*- Mode: objc -*-

@import Cocoa;

@interface SharedCheckboxItem : NSObject <NSCoding, NSCopying>

@property(readonly) NSNumber* id;
@property(copy, readonly) NSString* name;
@property(copy, readonly) NSString* style;
@property(copy, readonly) NSString* identifier;
@property(readonly) NSInteger childrenCount;
@property(readonly) BOOL needsShowCheckbox;

- (instancetype)initWithName:(NSString*)name
                       style:(NSString*)style
                  identifier:(NSString*)identifier
               childrenCount:(NSInteger)childrenCount
           needsShowCheckbox:(BOOL)needsShowCheckbox;

@end

@interface SharedCheckboxTree : NSObject <NSCoding, NSCopying>

@property NSNumber* id;
@property(copy) NSArray* children;

- (instancetype)initWithId:(NSNumber*)id
                  children:(NSArray*)children;

@end
