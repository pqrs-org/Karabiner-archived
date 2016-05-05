// -*- Mode: objc -*-

@import Cocoa;

@interface ParameterItem : NSObject <NSCoding, NSCopying>

- (instancetype)initWithName:(const char*)name
                  identifier:(const char*)identifier
                defaultValue:(int)defaultValue
                        step:(int)step
                    baseUnit:(const char*)baseUnit;

@property(copy, readonly) NSString* name;
@property(copy, readonly) NSString* identifier;
@property(readonly) NSInteger defaultValue;
@property(readonly) NSInteger step;
@property(copy, readonly) NSString* baseUnit;

@end

@interface ParameterTree : NSObject <NSCoding, NSCopying>

@property(readonly) ParameterItem* node;
@property(copy, readonly) NSArray* children;

- (instancetype)initWithItem:(ParameterItem*)node
                    children:(NSArray*)children;

@end
