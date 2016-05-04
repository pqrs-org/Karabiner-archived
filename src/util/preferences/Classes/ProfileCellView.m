#import "ProfileCellView.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ProfileTableView.h"

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

    [self.preferencesModel renameProfile:self.profileIndex name:self.textField.stringValue];
    [self.preferencesWindowController savePreferencesModel];
  }
}

- (IBAction)deleteProfile:(id)sender {
  [self.preferencesModel deleteProfile:self.profileIndex];
  [self.preferencesWindowController savePreferencesModel];
  [self.profileTableView reloadData];
}

@end
