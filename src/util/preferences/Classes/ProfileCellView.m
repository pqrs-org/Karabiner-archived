#import "ProfileCellView.h"
#import "PreferencesModel.h"
#import "ProfileTableView.h"
#import "SharedPreferencesManager.h"

@implementation ProfileCellView

- (void)prepareForReuse {
  [super prepareForReuse];

  self.profileIndex = -1;
}

- (IBAction)nameChanged:(id)sender {
  if (self.profileIndex >= 0) {
    if ([self.textField.stringValue length] == 0) {
      self.textField.stringValue = @"No Name";
    }

    [self.sharedPreferencesManager.pm renameProfile:self.profileIndex name:self.textField.stringValue];
    [self.sharedPreferencesManager save];
  }
}

- (IBAction)deleteProfile:(id)sender {
  [self.sharedPreferencesManager.pm deleteProfile:self.profileIndex];
  [self.sharedPreferencesManager save];
  [self.profileTableView reloadData];
}

@end
