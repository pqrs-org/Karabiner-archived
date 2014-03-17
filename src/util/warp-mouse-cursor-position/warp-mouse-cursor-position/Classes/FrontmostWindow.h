// -*- Mode: objc -*-

@import Cocoa;

@interface FrontmostWindow : NSObject

- (instancetype) init;

@property (readonly) NSDictionary* window;
@property (readonly) CGRect windowBounds;
@property (readonly) NSString* bundleIdentifier;

@end
