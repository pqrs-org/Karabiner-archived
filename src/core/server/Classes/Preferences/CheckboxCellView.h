// -*- Mode: objc -*-

@import Cocoa;

@class CheckboxBackgroundView;
@class CheckboxOutlineViewDelegate;

@interface CheckboxCellView : NSTableCellView
@property(weak) IBOutlet NSButton* checkbox;
@property(weak) IBOutlet NSLayoutConstraint* labelLeadingSpace;
@property(weak) IBOutlet NSLayoutConstraint* labelTopSpace;
@property(weak) IBOutlet NSLayoutConstraint* labelBottomSpace;
@property(weak) IBOutlet CheckboxOutlineViewDelegate* checkboxOutlineViewDelegate;
@property CheckboxBackgroundView* backgroundView;
@property NSString* settingIdentifier;

- (void)addLayoutConstraint:(NSView*)subview top:(CGFloat)top bottom:(CGFloat)bottom leading:(CGFloat)leading trailing:(CGFloat)trailing;
@end
