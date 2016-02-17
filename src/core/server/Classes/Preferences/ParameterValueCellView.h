// -*- Mode: objc -*-

@import Cocoa;

@class ServerObjects;

@interface ParameterValueCellView : NSTableCellView
@property(weak) IBOutlet NSStepper* stepper;
@property(weak) ServerObjects* serverObjects;
@property NSString* settingIdentifier;
@end
