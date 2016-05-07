#import "SharedPreferencesManager.h"
#import "ServerClient.h"

@interface SharedPreferencesManager ()

@property(weak) IBOutlet ServerClient* client;

@end

@implementation SharedPreferencesManager

- (void)load {
  self.pm = [self.client.proxy preferencesModel];
}

- (void)save {
  [self.client.proxy savePreferencesModel:self.pm processIdentifier:[NSProcessInfo processInfo].processIdentifier];
}

- (void)setValue:(int)newval forName:(NSString*)name {
  [self.client.proxy setValue:newval forName:name];
}

@end
