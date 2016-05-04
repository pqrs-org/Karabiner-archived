// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesModel;
@class PreferencesWindowController;
@class ServerClient;

@interface ParameterDiffCellView : NSTableCellView

@property(weak) PreferencesModel* preferencesModel;
@property(weak) PreferencesWindowController* preferencesWindowController;
@property(weak) ServerClient* client;
@property(copy) NSString* settingIdentifier;
@property NSInteger defaultValue;

- (void)setObserver;
- (void)updateValue;

@end
