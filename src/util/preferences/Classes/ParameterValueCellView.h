// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesClient;

@interface ParameterValueCellView : NSTableCellView

@property(weak) IBOutlet NSStepper* stepper;
@property(weak) PreferencesClient* preferencesClient;
@property(copy) NSString* settingIdentifier;

@end
