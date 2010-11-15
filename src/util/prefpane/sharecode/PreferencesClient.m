#import "PreferencesClient.h"

@implementation KeyRemap4MacBookPreferencesClient

@synthesize proxy;

- (id) init
{
  [super init];

  proxy = [[NSConnection rootProxyForConnectionWithRegisteredName:@"org.pqrs.KeyRemap4MacBook" host:nil] retain];

  return self;
}

- (void) dealloc
{
  if (proxy) {
    [proxy release];
  }
  [super dealloc];
}

@end
