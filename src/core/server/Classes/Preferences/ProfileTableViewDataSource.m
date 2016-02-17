#import "PreferencesManager.h"
#import "ProfileTableViewDataSource.h"
#import "ServerObjects.h"

@interface ProfileTableViewDataSource ()
@property(weak) IBOutlet ServerObjects* serverObjects;
@end

@implementation ProfileTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return (NSInteger)([self.serverObjects.preferencesManager configlist_count]);
}

@end
