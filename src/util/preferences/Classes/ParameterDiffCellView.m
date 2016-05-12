#import "ParameterDiffCellView.h"
#import "NotificationKeys.h"
#import "PreferencesClient.h"
#import "PreferencesModel.h"
#import "weakify.h"

@implementation ParameterDiffCellView

- (void)observer_ParameterValueChanged:(NSNotification*)notification {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self updateValue];
  });
}

- (void)dealloc {
  [self removeObserver];
}

- (void)prepareForReuse {
  [self removeObserver];
}

- (void)removeObserver {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)setObserver {
  [self removeObserver];

  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(observer_ParameterValueChanged:)
                                               name:kParameterValueChangedNotification
                                             object:nil];
}

- (void)updateValue {
  if (!self.settingIdentifier) {
    return;
  }

  NSInteger value = [self.preferencesClient.pm value:self.settingIdentifier];
  if (value == self.defaultValue) {
    self.textField.stringValue = @"";
  } else {
    NSInteger diff = value - self.defaultValue;
    self.textField.stringValue = [NSString stringWithFormat:@"YES (%@%ld)", diff > 0 ? @"+" : @"", diff];
  }
}

@end
