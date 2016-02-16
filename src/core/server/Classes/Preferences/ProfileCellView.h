// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesManager;

@interface ProfileCellView : NSTableCellView
@property(weak) IBOutlet NSButton* deleteButton;
@property(weak) IBOutlet NSTextField* statusLabel;
@property(weak) PreferencesManager* preferencesManager;
@property NSInteger profileIndex;
@end
