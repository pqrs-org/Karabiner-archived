#import "KeyRemap4MacBookClient.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"

@implementation org_pqrs_KeyRemap4MacBook_Client

- (id<org_pqrs_KeyRemap4MacBook_Protocol>) proxy
{
  if (! proxy_) {
    [self refresh_connection];
  }
  return [[proxy_ retain] autorelease];
}

- (void) refresh_connection
{
  [proxy_ release];
  proxy_ = [[NSConnection rootProxyForConnectionWithRegisteredName:kKeyRemap4MacBookConnectionName host:nil] retain];
  [proxy_ setProtocolForProxy:@protocol(org_pqrs_KeyRemap4MacBook_Protocol)];
}

- (void) observer_NSConnectionDidDieNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   NSLog (@"observer_NSConnectionDidDieNotification is called");
                   [self refresh_connection];
                 });
}

- (void) distributedObserver_serverLaunched:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   NSLog (@"distributedObserver_serverLaunched is called");
                   [self refresh_connection];
                 });
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
                                                                  selector:@selector(distributedObserver_serverLaunched:)
                                                                      name:kKeyRemap4MacBookServerLaunchedNotification];

    [self refresh_connection];
  }

  return self;
}

- (void) dealloc
{
  // Call removeObserver first because observer may refresh connection.
  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter removeObserver:self];
  [[NSNotificationCenter defaultCenter] removeObserver:self];

  [proxy_ release];

  [super dealloc];
}

@end
