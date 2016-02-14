#import "NotificationKeys.h"
#import "ParameterValueCellView.h"
#import "PreferencesManager.h"

@interface ParameterValueCellView ()
@end

@implementation ParameterValueCellView

- (void)updatePreferences {
  NSDictionary* notificationUserInfo = @{
    kPreferencesChangedNotificationUserInfoKeyPreferencesChangedFromGUI : @YES,
  };

  [self.preferencesManager setValue:self.stepper.intValue
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
