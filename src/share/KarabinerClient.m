#import "KarabinerClient.h"
#import "SharedKeys.h"

@interface KarabinerClient ()

@property NSDistantObject<KarabinerProtocol>* connection;
@property dispatch_queue_t connectionQueue;

@end

@implementation KarabinerClient

- (NSDistantObject<KarabinerProtocol>*)proxy {
  dispatch_sync(self.connectionQueue, ^{
    if (!self.connection) {
      self.connection = (NSDistantObject<KarabinerProtocol>*)([NSConnection rootProxyForConnectionWithRegisteredName:kKarabinerConnectionName host:nil]);
      [self.connection setProtocolForProxy:@protocol(KarabinerProtocol)];
    }
  });
  return self.connection;
}

- (void)observer_NSConnectionDidDieNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    dispatch_sync(self.connectionQueue, ^{
      NSLog(@"observer_NSConnectionDidDieNotification is called");
      self.connection = nil;
    });
  });
}

- (id)init {
  self = [super init];

  if (self) {
    self.connectionQueue = dispatch_queue_create("org.pqrs.Karabiner.KarabinerClient.connectionQueue", NULL);

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_NSConnectionDidDieNotification:)
                                                 name:NSConnectionDidDieNotification
                                               object:nil];
  }

  return self;
}

- (void)dealloc {
  // Call removeObserver first because observer may refresh connection.
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end
