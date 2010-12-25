#import "KeyRemap4MacBookClient.h"

@implementation org_pqrs_KeyRemap4MacBook_Client

- (id) init
{
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_NSConnectionDidDieNotification:)
                                                 name:NSConnectionDidDieNotification
                                               object:nil];
    [self observer_NSConnectionDidDieNotification:nil];
  }

  return self;
}

- (void) dealloc
{
  [proxy_ release];

  [super dealloc];
}

- (void) observer_NSConnectionDidDieNotification:(NSNotification*)notification
{
  [proxy_ release];
  proxy_ = [[NSConnection rootProxyForConnectionWithRegisteredName:@"org.pqrs.KeyRemap4MacBook" host:nil] retain];
  [proxy_ setProtocolForProxy:@protocol(org_pqrs_KeyRemap4MacBook_Protocol)];
}

- (id) proxy
{
  if (! proxy_) {
    [self observer_NSConnectionDidDieNotification:nil];
  }
  return [[proxy_ retain] autorelease];
}

@end
