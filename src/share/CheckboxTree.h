// -*- Mode: objc -*-

@import Cocoa;

@interface CheckboxItem : NSObject <NSCoding, NSCopying>

- (instancetype)initWithName:(const char*)name style:(const char*)style identifier:(const char*)identifier;

@property(readonly) NSNumber* id;
@property(copy, readonly) NSString* name;
@property(copy, readonly) NSString* style;
@property(copy, readonly) NSString* identifier;
@property(readonly) BOOL needsShowCheckbox;

@end

@interface CheckboxTree : NSObject <NSCoding, NSCopying>

@property(readonly) CheckboxItem* node;
@property(copy, readonly) NSArray* children;

- (instancetype)initWithItem:(CheckboxItem*)node
                    children:(NSArray*)children;

@end
