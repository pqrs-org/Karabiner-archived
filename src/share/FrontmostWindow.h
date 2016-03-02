// -*- Mode: objc -*-

@import Cocoa;

@interface FrontmostWindow : NSObject

- (instancetype)init;

@property(readonly) CGRect windowBounds;
@property(copy, readonly) NSString* windowName;
@property(copy, readonly) NSString* bundleIdentifier;

@end
