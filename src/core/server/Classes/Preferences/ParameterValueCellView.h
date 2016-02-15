// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesManager;

@interface ParameterValueCellView : NSTableCellView
@property(weak) IBOutlet NSStepper* stepper;
@property(weak) PreferencesManager* preferencesManager;
@property NSString* settingIdentifier;
@end
