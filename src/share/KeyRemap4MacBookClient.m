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

- (void) distributedObserver_serverLaunched:(NSNotification*)notification
{
  // [NSAutoreleasePool drain] is never called from NSDistributedNotificationCenter.
  // Therefore, we need to make own NSAutoreleasePool.
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    NSLog(@"distributedObserver_serverLaunched called");
    [self refresh_connection];
  }
  [pool drain];
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

  [proxy release];

  [super dealloc];
}

@end
