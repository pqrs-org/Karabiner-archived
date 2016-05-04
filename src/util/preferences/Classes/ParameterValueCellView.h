// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesModel;
@class PreferencesWindowController;
@class ServerClient;

@interface ParameterValueCellView : NSTableCellView

@property(weak) IBOutlet NSStepper* stepper;
@property(weak) PreferencesModel* preferencesModel;
@property(weak) PreferencesWindowController* preferencesWindowController;
@property(weak) ServerClient* client;
@property(copy) NSString* settingIdentifier;

@end
