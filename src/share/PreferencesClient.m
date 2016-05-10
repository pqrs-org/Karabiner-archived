#import "PreferencesClient.h"
#import "PreferencesModel.h"
#import "ServerClient.h"

@interface PreferencesClient ()

@property(weak) IBOutlet ServerClient* client;

@end

@implementation PreferencesClient

- (void)load {
  self.pm = [self.client.proxy preferencesModel];
}

- (void)save {
  [self.client.proxy savePreferencesModel:self.pm processIdentifier:[NSProcessInfo processInfo].processIdentifier];
}

- (void)setValue:(int)newval forName:(NSString*)name {
  if ([self.pm setValue:newval forName:name]) {
    [self save];
    [self.client.proxy updateKextValue:name];
  }
}

@end
