#import "KeyRemap4MacBookClient.h"
#import "KeyRemap4MacBookKeys.h"

@implementation KeyRemap4MacBookClient

- (id<KeyRemap4MacBookProtocol>) proxy
{
  @synchronized(self) {
    if (! proxy_) {
      [proxy_ release];
      proxy_ = [[NSConnection rootProxyForConnectionWithRegisteredName:kKeyRemap4MacBookConnectionName host:nil] retain];
      [proxy_ setProtocolForProxy:@protocol(KeyRemap4MacBookProtocol)];
    }
    return [[proxy_ retain] autorelease];
  }
}

- (void) observer_NSConnectionDidDieNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      NSLog(@"observer_NSConnectionDidDieNotification is called");
      [proxy_ release];
      proxy_ = nil;
    };
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
  }

  return self;
}

- (void) dealloc
{
  // Call removeObserver first because observer may refresh connection.
  [[NSNotificationCenter defaultCenter] removeObserver:self];

  [proxy_ release];

  [super dealloc];
}

@end
