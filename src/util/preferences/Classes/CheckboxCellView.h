// -*- Mode: objc -*-

@import Cocoa;

@class CheckboxBackgroundView;
@class ServerClient;

@interface CheckboxCellView : NSTableCellView

@property(weak) IBOutlet NSLayoutConstraint* labelLeadingSpace;
@property(weak) IBOutlet NSLayoutConstraint* labelTopSpace;
@property(weak) IBOutlet NSLayoutConstraint* labelBottomSpace;
@property(weak) ServerClient* client;
@property CheckboxBackgroundView* backgroundView;
@property NSButton* checkbox;
@property(copy) NSString* settingIdentifier;

- (void)addLayoutConstraint:(NSView*)subview top:(CGFloat)top bottom:(CGFloat)bottom leading:(CGFloat)leading trailing:(CGFloat)trailing;
- (void)toggle;
- (void)valueChanged:(id)sender;

@end
