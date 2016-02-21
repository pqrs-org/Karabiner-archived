#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "ProfileCellView.h"
#import "ServerObjects.h"

@implementation ProfileCellView

- (void)prepareForReuse {
  [super prepareForReuse];

  self.profileIndex = -1;
}

- (IBAction)nameChanged:(id)sender {
  if (self.profileIndex >= 0) {
    NSDictionary* notificationUserInfo = @{
      kPreferencesChangedNotificationUserInfoKeyPreferencesChangedFromGUI : @YES,
    };
    [self.serverObjects.preferencesManager configlist_setName:self.profileIndex
                                                         name:self.textField.stringValue
                                         notificationUserInfo:notificationUserInfo];
  }
}

- (IBAction)deleteProfile:(id)sender {
  if (self.profileIndex != [self.serverObjects.preferencesManager configlist_selectedIndex]) {
    [self.serverObjects.preferencesManager configlist_delete:self.profileIndex];
  }
}

@end
