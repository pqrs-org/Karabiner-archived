// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesManager;

@interface ParameterDiffCellView : NSTableCellView
@property(weak) PreferencesManager* preferencesManager;
@property NSString* settingIdentifier;
@property NSInteger defaultValue;

- (void)setObserver;
- (void)updateValue;
@end
