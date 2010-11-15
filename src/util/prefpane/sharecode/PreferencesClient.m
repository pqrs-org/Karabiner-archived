#import "PreferencesClient.h"

@implementation org_pqrs_KeyRemap4MacBook_PreferencesClient

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
