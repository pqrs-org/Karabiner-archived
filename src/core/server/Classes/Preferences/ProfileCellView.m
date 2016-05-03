#import "ProfileCellView.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"

@implementation ProfileCellView

- (void)prepareForReuse {
  [super prepareForReuse];

  self.profileIndex = -1;
}

- (IBAction)nameChanged:(id)sender {
}

- (IBAction)deleteProfile:(id)sender {
}

@end
