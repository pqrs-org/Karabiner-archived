// -*- Mode: objc -*-

@import Cocoa;

@interface CheckboxOutlineViewDataSource : NSObject <NSOutlineViewDataSource>

- (void)setup;
- (BOOL)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string;
- (void)clearFilterCondition;

@end
