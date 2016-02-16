#import "PreferencesManager.h"
#import "ProfileTableViewDataSource.h"

@interface ProfileTableViewDataSource ()
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@end

@implementation ProfileTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return (NSInteger)([self.preferencesManager configlist_count]);
}

@end
