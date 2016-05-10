// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesClient;

@interface ParameterDiffCellView : NSTableCellView

@property(weak) PreferencesClient* preferencesClient;
@property(copy) NSString* settingIdentifier;
@property NSInteger defaultValue;

- (void)setObserver;
- (void)updateValue;

@end
