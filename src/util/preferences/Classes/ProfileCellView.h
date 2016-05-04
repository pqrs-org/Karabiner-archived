// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesModel;
@class PreferencesWindowController;
@class ProfileTableView;

@interface ProfileCellView : NSTableCellView

@property(weak) IBOutlet NSButton* deleteButton;
@property(weak) IBOutlet NSTextField* statusLabel;
@property(weak) PreferencesModel* preferencesModel;
@property(weak) PreferencesWindowController* preferencesWindowController;
@property(weak) ProfileTableView* profileTableView;
@property NSInteger profileIndex;

@end
