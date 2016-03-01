// -*- Mode: objc -*-

@import Cocoa;

@class ServerObjects;

@interface ProfileCellView : NSTableCellView

@property(weak) IBOutlet NSButton* deleteButton;
@property(weak) IBOutlet NSTextField* statusLabel;
@property(weak) ServerObjects* serverObjects;
@property NSInteger profileIndex;

@end
