#import "CheckboxBackgroundView.h"
#import "CheckboxCellView.h"
#import "CheckboxOutlineViewDelegate.h"
#import "NotificationKeys.h"
#import "PreferencesManager.h"

@implementation CheckboxCellView

- (void)addLayoutConstraint:(NSView*)subview top:(CGFloat)top bottom:(CGFloat)bottom leading:(CGFloat)leading trailing:(CGFloat)trailing {
  [self addConstraint:[NSLayoutConstraint constraintWithItem:subview
                                                   attribute:NSLayoutAttributeTop
                                                   relatedBy:NSLayoutRelationEqual
                                                      toItem:self
                                                   attribute:NSLayoutAttributeTop
                                                  multiplier:1.0
                                                    constant:top]];

  [self addConstraint:[NSLayoutConstraint constraintWithItem:subview
                                                   attribute:NSLayoutAttributeBottom
                                                   relatedBy:NSLayoutRelationEqual
                                                      toItem:self
                                                   attribute:NSLayoutAttributeBottom
                                                  multiplier:1.0
                                                    constant:bottom]];

  [self addConstraint:[NSLayoutConstraint constraintWithItem:subview
                                                   attribute:NSLayoutAttributeLeading
                                                   relatedBy:NSLayoutRelationEqual
                                                      toItem:self
                                                   attribute:NSLayoutAttributeLeading
                                                  multiplier:1.0
                                                    constant:leading]];

  [self addConstraint:[NSLayoutConstraint constraintWithItem:subview
                                                   attribute:NSLayoutAttributeTrailing
                                                   relatedBy:NSLayoutRelationEqual
                                                      toItem:self
                                                   attribute:NSLayoutAttributeTrailing
                                                  multiplier:1.0
                                                    constant:trailing]];
}

- (void)prepareForReuse {
  [super prepareForReuse];

  [self.backgroundView removeFromSuperview];
  self.backgroundView = nil;

  [self.checkbox removeFromSuperview];
  self.checkbox = nil;
}

- (void)toggle {
  [self.checkbox setNextState];
  [self valueChanged:self];
}

- (void)valueChanged:(id)sender {
  if (self.checkbox) {
    int value = (self.checkbox.state == NSOnState);

    NSDictionary* notificationUserInfo = @{
      kPreferencesChangedNotificationUserInfoKeyPreferencesChangedFromGUI : @YES,
    };

    [self.checkboxOutlineViewDelegate.preferencesManager setValue:value
                                                          forName:self.settingIdentifier
                                                       tellToKext:YES
                                             notificationUserInfo:notificationUserInfo];
  } else {
    [self selectSelf];
    [self expandOrCollapseTree];
  }

  // Call makeFirstResponder via dispatch_async for safe.
  dispatch_async(dispatch_get_main_queue(), ^{
    [[self.checkboxOutlineViewDelegate.outlineView window] makeFirstResponder:self.checkboxOutlineViewDelegate.outlineView];
  });
}

- (void)selectSelf {
  NSInteger row = [self.checkboxOutlineViewDelegate.outlineView rowForView:self];
  if (row == -1) return;

  [self.checkboxOutlineViewDelegate.outlineView selectRowIndexes:[NSIndexSet indexSetWithIndex:(NSUInteger)(row)] byExtendingSelection:NO];
}

- (void)expandOrCollapseTree {
  NSInteger row = [self.checkboxOutlineViewDelegate.outlineView rowForView:self];
  if (row == -1) return;

  NSDictionary* item = [self.checkboxOutlineViewDelegate.outlineView itemAtRow:row];
  if (![self.checkboxOutlineViewDelegate.outlineView isExpandable:item]) {
    return;
  }

  if ([self.checkboxOutlineViewDelegate.outlineView isItemExpanded:item]) {
    [self.checkboxOutlineViewDelegate.outlineView collapseItem:item];
  } else {
    [self.checkboxOutlineViewDelegate.outlineView expandItem:item];
  }
}

@end
