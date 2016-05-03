#import "ParameterValueCellView.h"
#import "NotificationKeys.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"

@implementation ParameterValueCellView

- (void)updatePreferences {
  [self.serverObjects.serverForUserspace setValue:self.stepper.intValue forName:self.settingIdentifier];
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
