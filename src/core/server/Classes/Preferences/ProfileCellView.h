// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesManager;

@interface ProfileCellView : NSTableCellView
@property(weak) PreferencesManager* preferencesManager;
@property NSInteger profileIndex;
@end
