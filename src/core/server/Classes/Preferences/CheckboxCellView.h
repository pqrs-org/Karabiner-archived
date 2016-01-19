// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>

@interface CheckboxCellView : NSTableCellView
@property(weak) IBOutlet NSButton* checkbox;
@property NSString* settingIdentifier;

- (void) toggleCheckboxState;
@end
