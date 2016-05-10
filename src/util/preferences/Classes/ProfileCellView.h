// -*- Mode: objc -*-

@import Cocoa;

@class ProfileTableView;
@class PreferencesClient;

@interface ProfileCellView : NSTableCellView

@property(weak) IBOutlet NSButton* deleteButton;
@property(weak) IBOutlet NSTextField* statusLabel;
@property(weak) PreferencesClient* preferencesClient;
@property(weak) ProfileTableView* profileTableView;
@property NSInteger profileIndex;

@end
