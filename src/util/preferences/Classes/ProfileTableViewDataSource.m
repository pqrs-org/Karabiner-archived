#import "ProfileTableViewDataSource.h"
#import "PreferencesClient.h"
#import "PreferencesModel.h"

@interface ProfileTableViewDataSource ()

@property(weak) IBOutlet PreferencesClient* preferencesClient;

@end

@implementation ProfileTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return [self.preferencesClient.pm.profiles count];
}

@end
