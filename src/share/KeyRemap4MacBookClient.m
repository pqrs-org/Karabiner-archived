#import "KeyRemap4MacBookClient.h"

@implementation org_pqrs_KeyRemap4MacBook_Client

- (void) refresh_connection
{
  [proxy_ release];
  proxy_ = [[NSConnection rootProxyForConnectionWithRegisteredName:@"org.pqrs.KeyRemap4MacBook" host:nil] retain];
  [proxy_ setProtocolForProxy:@protocol(org_pqrs_KeyRemap4MacBook_Protocol)];
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
    [self observer_NSConnectionDidDieNotification:nil];

    NSString* observedObject = @"org.pqrs.KeyRemap4MacBook.notification";
    [[NSDistributedNotificationCenter defaultCenter] addObserver:self selector:@selector(observer_server_launched:) name:@"server_launched" object:observedObject];
  }

  return self;
}

- (void) dealloc
{
  [proxy_ release];

  [super dealloc];
}

- (id) proxy
{
  if (! proxy_) {
    [self observer_NSConnectionDidDieNotification:nil];
  }
  return [[proxy_ retain] autorelease];
}

@end
