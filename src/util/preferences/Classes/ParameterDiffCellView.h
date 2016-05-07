// -*- Mode: objc -*-

@import Cocoa;

@class SharedPreferencesManager;

@interface ParameterDiffCellView : NSTableCellView

@property(weak) SharedPreferencesManager* sharedPreferencesManager;
@property(copy) NSString* settingIdentifier;
@property NSInteger defaultValue;

- (void)setObserver;
- (void)updateValue;

@end
