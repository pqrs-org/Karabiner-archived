#import "ProfileTableViewDelegate.h"
#import "PreferencesClient.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ProfileCellView.h"
#import "ProfileTableView.h"

@interface ProfileTableViewDelegate ()

@property(weak) IBOutlet PreferencesClient* preferencesClient;
@property(weak) IBOutlet ProfileTableView* profileTableView;

@end

@implementation ProfileTableViewDelegate

- (NSView*)tableView:(NSTableView*)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  ProfileModel* profileModel = [self.preferencesClient.pm profile:row];
  if (!profileModel) {
    return nil;
  }

  ProfileCellView* result = [tableView makeViewWithIdentifier:@"ProfileCellView" owner:self];
  result.preferencesClient = self.preferencesClient;
  result.profileTableView = self.profileTableView;

  if (profileModel.name) {
    result.textField.stringValue = profileModel.name;
  }

  if (self.preferencesClient.pm.currentProfileIndex == row) {
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
