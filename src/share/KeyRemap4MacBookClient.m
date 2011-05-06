#import "KeyRemap4MacBookClient.h"
#import "KeyRemap4MacBookKeys.h"

@implementation org_pqrs_KeyRemap4MacBook_Client

@synthesize proxy;

- (void) refresh_connection
{
  [proxy release];
  proxy = [[NSConnection rootProxyForConnectionWithRegisteredName:@"org.pqrs.KeyRemap4MacBook.server" host:nil] retain];
  [proxy setProtocolForProxy:@protocol(org_pqrs_KeyRemap4MacBook_Protocol)];

  [[NSNotificationCenter defaultCenter] postNotificationName:@"KeyRemap4MacBookClient_connected" object:nil];
}

- (void) observer_NSConnectionDidDieNotification:(NSNotification*)notification
{
  [self refresh_connection];
}

- (void) observer_server_launched:(NSNotification*)notification
{
  NSLog(@"observer_server_launched called");
  [self refresh_connection];
}

- (id) init
{
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_NSConnectionDidDieNotification:)
                                                 name:NSConnectionDidDieNotification
                                               object:nil];

    [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                        selector:@selector(observer_server_launched:)
                                                            name:kKeyRemap4MacBookServerLaunchedNotification
                                                          object:kKeyRemap4MacBookNotificationKey];

    [self refresh_connection];
  }

  return self;
}

- (void) dealloc
{
  [proxy release];

  [super dealloc];
}

@end
