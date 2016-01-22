// -*- Mode: objc -*-

@import Cocoa;

@class CheckboxBackgroundView;
@class CheckboxOutlineViewDelegate;

@interface CheckboxCellView : NSTableCellView
@property(weak) IBOutlet NSButton* checkbox;
@property(weak) IBOutlet CheckboxBackgroundView* background;
@property(weak) IBOutlet NSLayoutConstraint* labelLeadingSpace;
@property(weak) IBOutlet NSLayoutConstraint* labelTopSpace;
@property(weak) IBOutlet NSLayoutConstraint* labelBottomSpace;
@property(weak) IBOutlet CheckboxOutlineViewDelegate* checkboxOutlineViewDelegate;
@property NSString* settingIdentifier;
@end
