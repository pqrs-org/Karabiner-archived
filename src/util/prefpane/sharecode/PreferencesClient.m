#import "PreferencesClient.h"

@implementation org_pqrs_KeyRemap4MacBook_PreferencesClient

- (id) init
{
  [super init];

  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(observer_NSConnectionDidDieNotification:)
                                               name:NSConnectionDidDieNotification
                                             object:nil];
  [self observer_NSConnectionDidDieNotification:nil];

  return self;
}

- (void) dealloc
{
  if (proxy_) {
    [proxy_ release];
  }
  [super dealloc];
}

- (void) observer_NSConnectionDidDieNotification:(NSNotification*)notification
{
  if (proxy_) {
    [proxy_ release];
  }
  proxy_ = [[NSConnection rootProxyForConnectionWithRegisteredName:@"org.pqrs.KeyRemap4MacBook" host:nil] retain];
}

- (id) proxy
{
  if (! proxy_) {
    [self observer_NSConnectionDidDieNotification:nil];
  }
  return proxy_;
}

@end
