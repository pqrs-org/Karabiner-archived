// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>

@interface OutlineViewDataSourceCheckbox : NSObject <NSOutlineViewDataSource>

- (void)load:(BOOL)force;
- (void)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string;

@end
