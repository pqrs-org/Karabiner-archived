#import "ProfileCellView.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"

@implementation ProfileCellView

- (void)prepareForReuse {
  [super prepareForReuse];

  self.profileIndex = -1;
}

- (IBAction)nameChanged:(id)sender {
  if (self.profileIndex >= 0) {
    if ([self.textField.stringValue length] == 0) {
      self.textField.stringValue = @"No Name";
    }

    [self.serverObjects.serverForUserspace configlist_setName:self.profileIndex
                                                         name:self.textField.stringValue];
  }
}

- (IBAction)deleteProfile:(id)sender {
  [self.serverObjects.serverForUserspace configlist_delete:self.profileIndex];
}

@end
