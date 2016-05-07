#import "ParameterDiffCellView.h"
#import "NotificationKeys.h"
#import "PreferencesModel.h"
#import "SharedPreferencesManager.h"

@implementation ParameterDiffCellView

- (void)observer_ParameterValueChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
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

  NSInteger value = [self.sharedPreferencesManager.pm value:self.settingIdentifier defaultValue:self.defaultValue];
  if (value == self.defaultValue) {
    self.textField.stringValue = @"";
  } else {
    NSInteger diff = value - self.defaultValue;
    self.textField.stringValue = [NSString stringWithFormat:@"YES (%@%ld)", diff > 0 ? @"+" : @"", diff];
  }
}

@end
