#import "KeyRemap4MacBookClient.h"

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

    NSString* observedObject = @"org.pqrs.KeyRemap4MacBook.notification";
    [[NSDistributedNotificationCenter defaultCenter] addObserver:self selector:@selector(observer_server_launched:) name:@"server_launched" object:observedObject];

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
