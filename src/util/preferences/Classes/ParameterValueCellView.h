// -*- Mode: objc -*-

@import Cocoa;

@class SharedPreferencesManager;

@interface ParameterValueCellView : NSTableCellView

@property(weak) IBOutlet NSStepper* stepper;
@property(weak) SharedPreferencesManager* sharedPreferencesManager;
@property(copy) NSString* settingIdentifier;

@end
