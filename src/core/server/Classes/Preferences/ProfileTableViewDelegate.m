#import "PreferencesManager.h"
#import "PreferencesWindowController.h"
#import "ProfileCellView.h"
#import "ProfileTableViewDelegate.h"
#import "ServerObjects.h"

@interface ProfileTableViewDelegate ()

@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;

@end

@implementation ProfileTableViewDelegate

- (NSView*)tableView:(NSTableView*)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  ProfileCellView* result = [tableView makeViewWithIdentifier:@"ProfileCellView" owner:self];
  result.serverObjects = self.preferencesWindowController.serverObjects;

  NSString* name = [self.preferencesWindowController.serverObjects.preferencesManager configlist_name:row];
  if (name) {
    result.textField.stringValue = name;
  }

  if ([self.preferencesWindowController.serverObjects.preferencesManager configlist_selectedIndex] == row) {
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
