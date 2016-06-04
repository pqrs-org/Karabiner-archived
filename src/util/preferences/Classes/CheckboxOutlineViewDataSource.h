// -*- Mode: objc -*-

@import Cocoa;

@class CheckboxTree;

@interface CheckboxOutlineViewDataSource : NSObject <NSOutlineViewDataSource>

@property(readonly) CheckboxTree* dataSource;
@property(readonly) CheckboxTree* fullDataSource;

- (void)setup;
- (void)clear;
- (BOOL)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string;
- (void)clearFilterCondition;

@end
