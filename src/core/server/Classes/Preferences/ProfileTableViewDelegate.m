#import "ProfileTableViewDelegate.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ProfileCellView.h"
#import "ProfileTableViewDataSource.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"

@interface ProfileTableViewDelegate ()

@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;
@property(weak) IBOutlet ProfileTableViewDataSource* profileTableViewDataSource;

@end

@implementation ProfileTableViewDelegate

- (NSView*)tableView:(NSTableView*)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  NSArray* dataSource = self.profileTableViewDataSource.dataSource;
  if (row < 0 || row >= (NSInteger)([dataSource count])) {
    return nil;
  }

  ProfileCellView* result = [tableView makeViewWithIdentifier:@"ProfileCellView" owner:self];
  result.serverObjects = self.preferencesWindowController.serverObjects;

  NSString* name = dataSource[row][@"name"];
  if (name) {
    result.textField.stringValue = name;
  }

  if (self.preferencesModel.selectedProfileIndex == row) {
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
