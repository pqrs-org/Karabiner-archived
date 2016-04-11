#import "ParameterValueCellView.h"
#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "ServerObjects.h"

@implementation ParameterValueCellView

- (void)updatePreferences {
  NSDictionary* notificationUserInfo = @{
    kPreferencesChangedNotificationUserInfoKeyPreferencesChangedFromGUI : @YES,
  };

  [self.serverObjects.preferencesManager setValue:self.stepper.intValue
                                          forName:self.settingIdentifier
                                       tellToKext:YES
                             notificationUserInfo:notificationUserInfo];
}

- (IBAction)valueChangedByStepper:(id)sender {
  self.textField.stringValue = [NSString stringWithFormat:@"%ld", self.stepper.integerValue];
  [self updatePreferences];
}

- (IBAction)valueChangedByTextField:(id)sender {
  self.stepper.integerValue = [self.textField.stringValue integerValue];
  [self updatePreferences];
}

@end
