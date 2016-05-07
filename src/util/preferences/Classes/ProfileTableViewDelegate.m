#import "ProfileTableViewDelegate.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ProfileCellView.h"
#import "ProfileTableView.h"
#import "SharedPreferencesManager.h"

@interface ProfileTableViewDelegate ()

@property(weak) IBOutlet ProfileTableView* profileTableView;
@property(weak) IBOutlet SharedPreferencesManager* sharedPreferencesManager;

@end

@implementation ProfileTableViewDelegate

- (NSView*)tableView:(NSTableView*)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  ProfileModel* profileModel = [self.sharedPreferencesManager.pm profile:row];
  if (!profileModel) {
    return nil;
  }

  ProfileCellView* result = [tableView makeViewWithIdentifier:@"ProfileCellView" owner:self];
  result.sharedPreferencesManager = self.sharedPreferencesManager;
  result.profileTableView = self.profileTableView;

  if (profileModel.name) {
    result.textField.stringValue = profileModel.name;
  }

  if (self.sharedPreferencesManager.pm.currentProfileIndex == row) {
    result.deleteButton.hidden = YES;
    result.statusLabel.hidden = NO;
  } else {
    result.deleteButton.hidden = NO;
    result.statusLabel.hidden = YES;
  }

  result.profileIndex = row;

  return result;
}

@end
