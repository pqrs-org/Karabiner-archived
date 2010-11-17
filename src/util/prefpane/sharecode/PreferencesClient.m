#import "PreferencesClient.h"

@implementation org_pqrs_KeyRemap4MacBook_PreferencesClient

@synthesize proxy;

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

- (void) observer_NSConnectionDidDieNotification:(NSNotification*)notification
{
  if (proxy) {
    [proxy release];
  }
  proxy = [[NSConnection rootProxyForConnectionWithRegisteredName:@"org.pqrs.KeyRemap4MacBook" host:nil] retain];
}

- (void) dealloc
{
  if (proxy) {
    [proxy release];
  }
  [super dealloc];
}

@end
