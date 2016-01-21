// -*- Mode: objc -*-

@import Cocoa;

@interface CheckboxOutlineViewDataSource : NSObject <NSOutlineViewDataSource>

- (void)load:(BOOL)force;
- (void)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string;
+ (BOOL)isCheckbox:(NSString*)identifier;

@end
