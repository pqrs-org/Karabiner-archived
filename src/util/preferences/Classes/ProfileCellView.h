// -*- Mode: objc -*-

@import Cocoa;

@class ProfileTableView;
@class SharedPreferencesManager;

@interface ProfileCellView : NSTableCellView

@property(weak) IBOutlet NSButton* deleteButton;
@property(weak) IBOutlet NSTextField* statusLabel;
@property(weak) SharedPreferencesManager* sharedPreferencesManager;
@property(weak) ProfileTableView* profileTableView;
@property NSInteger profileIndex;

@end
