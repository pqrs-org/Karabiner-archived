#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "ProfileCellView.h"

@implementation ProfileCellView

- (IBAction)nameChanged:(id)sender {
  [self.preferencesManager configlist_setName:self.profileIndex name:self.textField.stringValue];
}

- (IBAction)deleteProfile:(id)sender {
  if (self.profileIndex != [self.preferencesManager configlist_selectedIndex]) {
    [self.preferencesManager configlist_delete:self.profileIndex];
  }
}

@end
