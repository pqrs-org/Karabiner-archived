#import "KeyRemap4MacBookClient.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"

@implementation org_pqrs_KeyRemap4MacBook_Client

@synthesize proxy;

- (void) refresh_connection
{
  [proxy release];
  proxy = [[NSConnection rootProxyForConnectionWithRegisteredName:kKeyRemap4MacBookConnectionName host:nil] retain];
  [proxy setProtocolForProxy:@protocol(org_pqrs_KeyRemap4MacBook_Protocol)];
}

- (void) observer_NSConnectionDidDieNotification:(NSNotification*)notification
{
  [self refresh_connection];
}

- (void) observer_serverLaunched:(NSNotification*)notification
{
  NSLog(@"observer_serverLaunched called");
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

    [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                  selector:@selector(observer_serverLaunched:)
                                                                      name:kKeyRemap4MacBookServerLaunchedNotification];

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
