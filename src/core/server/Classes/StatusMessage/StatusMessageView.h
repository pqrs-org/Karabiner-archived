// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesModel;

@interface StatusMessageView : NSView

@property(weak) IBOutlet NSTextField* message;
@property(weak) PreferencesModel* preferencesModel;

- (void)updateMessage:(NSString*)message;
- (void)updateWindowFrame:(NSScreen*)screen;

@end
