#import "PreferencesClient.h"
#import "PreferencesModel.h"
#import "ServerClient.h"

@interface PreferencesClient ()

@property(weak) IBOutlet ServerClient* client;

@end

@implementation PreferencesClient

- (void)load {
  self.pm = [self.client preferencesModel];
}

- (void)save {
  if (self.pm) {
    [self.client savePreferencesModel:self.pm processIdentifier:[NSProcessInfo processInfo].processIdentifier];
  }
}

- (void)setValue:(int)newval forIdentifier:(NSString*)identifier {
  if ([self.pm setValue:newval forIdentifier:identifier]) {
    [self save];
    [self.client updateKextValue:identifier];
  }
}

@end
