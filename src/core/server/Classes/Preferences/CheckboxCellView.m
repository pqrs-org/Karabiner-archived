#import "CheckboxBackgroundView.h"
#import "CheckboxCellView.h"
#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "ServerObjects.h"

@implementation CheckboxCellView

- (void)prepareForReuse {
  [super prepareForReuse];

  [self.backgroundView removeFromSuperview];
  self.backgroundView = nil;

  [self.checkbox removeFromSuperview];
  self.checkbox = nil;
}

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

    [self.serverObjects.preferencesManager setValue:value
                                            forName:self.settingIdentifier
                                         tellToKext:YES
                               notificationUserInfo:notificationUserInfo];
  }
}

@end
