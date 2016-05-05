// -*- Mode: objc -*-

@import Cocoa;

@class CheckboxTree;

@interface CheckboxOutlineViewDataSource : NSObject <NSOutlineViewDataSource>

@property(readonly) CheckboxTree* dataSource;
@property(readonly) CheckboxTree* fullDataSource;

- (void)setup;
- (BOOL)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string;
- (void)clearFilterCondition;

@end
