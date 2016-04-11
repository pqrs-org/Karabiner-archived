#import "ProfileTableViewDataSource.h"
#import "PreferencesManager.h"
#import "PreferencesWindowController.h"
#import "ServerObjects.h"

@interface ProfileTableViewDataSource ()

@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;

@end

@implementation ProfileTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return (NSInteger)([self.preferencesWindowController.serverObjects.preferencesManager configlist_count]);
}

@end
