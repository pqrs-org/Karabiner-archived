#import "ParameterValueCellView.h"
#import "NotificationKeys.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ServerClient.h"
#import "SharedPreferencesManager.h"

@implementation ParameterValueCellView

- (void)updatePreferences {
  [self.sharedPreferencesManager setValue:self.stepper.intValue forName:self.settingIdentifier];
  [[NSNotificationCenter defaultCenter] postNotificationName:kParameterValueChangedNotification object:nil];
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
