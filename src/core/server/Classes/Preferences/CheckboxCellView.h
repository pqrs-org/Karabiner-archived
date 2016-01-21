// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>

@class PreferencesManager;

@interface CheckboxCellView : NSTableCellView
@property(weak) IBOutlet NSButton* checkbox;
@property(weak) IBOutlet NSLayoutConstraint* labelLeadingSpace;
@property(weak) IBOutlet NSLayoutConstraint* labelTopSpace;
@property(weak) IBOutlet NSLayoutConstraint* labelBottomSpace;
@property NSString* settingIdentifier;
@property(weak) PreferencesManager* preferencesManager;

- (void)toggleCheckboxState;
@end
