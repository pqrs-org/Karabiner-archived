// -*- Mode: objc -*-

@import Cocoa;

@interface FrontmostWindow : NSObject

- (instancetype) init;

@property (readonly) CGRect windowBounds;
@property (readonly) NSString* windowName;
@property (readonly) NSString* bundleIdentifier;

@end
