// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesClient;
@class ServerClient;
@class ProfileTableView;

@interface ProfileCellView : NSTableCellView

@property(weak) IBOutlet NSButton* deleteButton;
@property(weak) IBOutlet NSTextField* statusLabel;
@property(weak) PreferencesClient* preferencesClient;
@property(weak) ProfileTableView* profileTableView;
@property(weak) ServerClient* client;
@property NSInteger profileIndex;

@end
