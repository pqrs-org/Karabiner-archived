#import "ParameterDiffCellView.h"
#import "NotificationKeys.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"

@implementation ParameterDiffCellView

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self updateValue];
  });
}

- (void)observer_ConfigXMLReloaded:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self updateValue];
  });
}

- (void)observer_PreferencesChanged:(NSNotification*)notification {
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
                                           selector:@selector(observer_ConfigListChanged:)
                                               name:kConfigListChangedNotification
                                             object:nil];

  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(observer_ConfigXMLReloaded:)
                                               name:kConfigXMLReloadedNotification
                                             object:nil];

  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(observer_PreferencesChanged:)
                                               name:kPreferencesChangedNotification
                                             object:nil];
}

- (void)updateValue {
  if (!self.settingIdentifier) {
    return;
  }

  NSInteger value = [self.serverObjects.serverForUserspace value:self.settingIdentifier];
  if (value == self.defaultValue) {
    self.textField.stringValue = @"";
  } else {
    NSInteger diff = value - self.defaultValue;
    self.textField.stringValue = [NSString stringWithFormat:@"YES (%@%ld)", diff > 0 ? @"+" : @"", diff];
  }
}

@end
