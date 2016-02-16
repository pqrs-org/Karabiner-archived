#import "PreferencesManager.h"
#import "ProfileCellView.h"
#import "ProfileTableViewDelegate.h"

@interface ProfileTableViewDelegate ()
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@end

@implementation ProfileTableViewDelegate

- (NSView*)tableView:(NSTableView*)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  ProfileCellView* result = [tableView makeViewWithIdentifier:@"ProfileCellView" owner:self];
  result.preferencesManager = self.preferencesManager;
  result.profileIndex = row;

  NSString* name = [self.preferencesManager configlist_name:row];
  if (name) {
    result.textField.stringValue = name;
  }

  if ([self.preferencesManager configlist_selectedIndex] == row) {
    result.deleteButton.hidden = YES;
    result.statusLabel.hidden = NO;
  } else {
    result.deleteButton.hidden = NO;
    result.statusLabel.hidden = YES;
  }

  return result;
}

@end
