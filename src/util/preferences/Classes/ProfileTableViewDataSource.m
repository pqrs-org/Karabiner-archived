#import "ProfileTableViewDataSource.h"
#import "PreferencesModel.h"

@interface ProfileTableViewDataSource ()

@property(weak) IBOutlet PreferencesModel* preferencesModel;

@end

@implementation ProfileTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return [self.preferencesModel.profiles count];
}

@end
