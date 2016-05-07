#import "ProfileTableViewDataSource.h"
#import "PreferencesModel.h"
#import "SharedPreferencesManager.h"

@interface ProfileTableViewDataSource ()

@property(weak) IBOutlet SharedPreferencesManager* sharedPreferencesManager;

@end

@implementation ProfileTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return [self.sharedPreferencesManager.pm.profiles count];
}

@end
