#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "ProfileCellView.h"
#import "ServerObjects.h"

@implementation ProfileCellView

- (IBAction)nameChanged:(id)sender {
  NSDictionary* notificationUserInfo = @{
    kPreferencesChangedNotificationUserInfoKeyPreferencesChangedFromGUI : @YES,
  };
  [self.serverObjects.preferencesManager configlist_setName:self.profileIndex
                                                       name:self.textField.stringValue
                                       notificationUserInfo:notificationUserInfo];
}

- (IBAction)deleteProfile:(id)sender {
  if (self.profileIndex != [self.serverObjects.preferencesManager configlist_selectedIndex]) {
    [self.serverObjects.preferencesManager configlist_delete:self.profileIndex];
  }
}

@end
