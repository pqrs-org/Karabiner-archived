// -*- Mode: objc -*-

@import Cocoa;

@interface ProfileTableViewDataSource : NSObject <NSTableViewDataSource>

@property(copy) NSArray* dataSource;

- (void)load:(BOOL)force;

@end
